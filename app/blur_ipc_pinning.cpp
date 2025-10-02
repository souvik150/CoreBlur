//
// Created by souvik on 10/2/25.
//

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>

#include "gaussian_blur.h"
#include "image_io.h"
#include "ipc_manager.h"
#include "shm_manager.h"
#include "tile_manager.h"

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

    // 🔹 Shared memory setup
    ShmManager shm("/coreblur_shm", image_size, true);
    auto* shm_ptr = static_cast<unsigned char*>(shm.data());
    memcpy(shm_ptr, input, image_size);

    GaussianBlur::Params params{kernel, sigma};
    GaussianBlur blur;

    // 🔹 IPCManager now handles: fork, core pinning, semaphore, join
    IPCManager ipc(num_tiles, "/coreblur_done_sem", shm_ptr);

    auto t0 = std::chrono::high_resolution_clock::now();

    auto tiles = TileManager::split(shm_ptr, width, height, channels, num_tiles);

    ipc.launchWorkers(tiles, [&](int start_row, int end_row, unsigned char* tile_ptr) {
        int tile_h = end_row - start_row;
        std::vector<unsigned char> local_out(static_cast<size_t>(tile_h) * width * channels);
        blur.processTile(tile_ptr, local_out.data(), width, tile_h, channels, params);
        std::memcpy(tile_ptr, local_out.data(), static_cast<size_t>(tile_h) * width * channels);
    });

    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = t1 - t0;
    std::cout << "Parallel blur completed in " << elapsed.count() << " ms\n";

    // 🔹 Output join
    std::vector<unsigned char> out_buf(image_size);
    TileManager::join(tiles, out_buf.data(), width, height, channels);

    if (!save_image_data(outputPath, out_buf.data(), width, height, channels)) {
        std::cerr << "Failed to save image\n";
    }

    shm.unlink();
    return 0;
}
