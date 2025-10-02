//
// Created by souvik on 10/2/25.
//
#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "shm_manager.h"
#include "tile_manager.h"
#include "image_io.h"
#include "gaussian_blur.h"

static const char* SEM_NAME = "/coreblur_done_sem";

int main(const int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input> <output> [num_tiles] [kernel] [sigma]\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    int num_tiles = (argc > 3) ? std::stoi(argv[3]) : 4;
    int kernel = (argc > 4) ? std::stoi(argv[4]) : 5;
    double sigma = (argc > 5) ? std::stod(argv[5]) : 1.5;

    int width = 0, height = 0, channels = 0;
    auto input = load_image_data(inputPath, width, height, channels);
    if (!input) { std::cerr << "Failed to load image\n"; return 1; }

    size_t image_size = static_cast<size_t>(width) * height * channels;

    ShmManager shm("/coreblur_shm", image_size, true);
    auto* shm_ptr = static_cast<unsigned char*>(shm.data());
    memcpy(shm_ptr, input, image_size);

    sem_unlink(SEM_NAME);
    sem_t* sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) { std::cerr << "sem_open failed\n"; return 1; }

    GaussianBlur::Params params{kernel, sigma};
    GaussianBlur blur;

    auto tiles = TileManager::split(shm_ptr, width, height, channels, num_tiles);

    auto t0 = std::chrono::high_resolution_clock::now();

    std::vector<pid_t> pids;
    for (auto & tile : tiles) {
        pid_t pid = fork();
        if (pid == 0) {
            int start = tile.start_row;
            int end = tile.end_row;
            int tile_h = end - start;
            unsigned char* tile_ptr = tile.data;
            std::vector<unsigned char> local_out(static_cast<size_t>(tile_h) * width * channels);
            blur.processTile(tile_ptr, local_out.data(), width, tile_h, channels, params);
            std::memcpy(tile_ptr, local_out.data(), static_cast<size_t>(tile_h) * width * channels);
            sem_post(sem);
            _exit(0);
        } else if (pid > 0) {
            pids.push_back(pid);
        } else {
            std::cerr << "fork failed\n";
            sem_close(sem);
            shm.unlink();
            return 1;
        }
    }

    for (size_t i = 0; i < tiles.size(); ++i) sem_wait(sem);

    for (pid_t pid : pids) waitpid(pid, nullptr, 0);

    const auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = t1 - t0;
    std::cout << "Parallel blur completed in " << elapsed.count() << " ms\n";

    std::vector<unsigned char> out_buf(image_size);
    TileManager::join(tiles, out_buf.data(), width, height, channels);

    if (!save_image_data(outputPath, out_buf.data(), width, height, channels)) {
        std::cerr << "Failed to save image\n";
    }

    sem_close(sem);
    sem_unlink(SEM_NAME);
    shm.unlink();
    return 0;
}
