//
// Created by souvik on 10/2/25.
//


#include "ipc_manager.h"

#include <iostream>
#include <sched.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

#include "tile_manager.h"

IPCManager::IPCManager(int num_workers,
                       const std::string& sem_name,
                       unsigned char* shm_ptr,
                       CoreStrategy strategy)
    : num_workers(num_workers),
      sem_name(sem_name),
      shm_ptr(shm_ptr),
      sem(nullptr),
      core_strategy(strategy)
{
    sem_unlink(sem_name.c_str());
    sem = sem_open(sem_name.c_str(), O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
}

IPCManager::~IPCManager() {
    if (sem) {
        sem_close(sem);
        sem_unlink(sem_name.c_str());
    }
}

void IPCManager::launchWorkers(const std::vector<Tile>& tiles,
                               std::function<void(int, int, unsigned char*)> worker_func)
{
    if (tiles.empty()) {
        std::cerr << "IPCManager::launchWorkers: empty tiles\n";
        return;
    }

    const size_t worker_count = tiles.size();
    std::vector<int> p_cores = getPCores();
    std::vector<int> e_cores = getECores();

    workers.clear();
    workers.reserve(worker_count);

    for (size_t i = 0; i < worker_count; ++i) {
        pid_t pid = fork();
        if (pid == 0) { // child
            // choose core id according to strategy, robust to core vector sizes
            int core_id = -1;

            if (core_strategy == CoreStrategy::P_ONLY && !p_cores.empty()) {
                core_id = p_cores[i % p_cores.size()];
            } else if (core_strategy == CoreStrategy::E_ONLY && !e_cores.empty()) {
                core_id = e_cores[i % e_cores.size()];
            } else if (core_strategy == CoreStrategy::MIXED) {
                // alternate: even child -> P, odd child -> E (round-robin within each)
                if ((i % 2) == 0 && !p_cores.empty()) {
                    core_id = p_cores[(i/2) % p_cores.size()];
                } else if (!e_cores.empty()) {
                    core_id = e_cores[(i/2) % e_cores.size()];
                } else if (!p_cores.empty()) {
                    core_id = p_cores[i % p_cores.size()];
                }
            } else {
                // fallback: just round robin across all online CPUs
                int total = sysconf(_SC_NPROCESSORS_ONLN);
                core_id = static_cast<int>(i % total);
            }

            if (core_id >= 0) {
                pinProcessToCore(core_id);

                // 🟢 Log from child process
                std::cout << "[Child " << getpid() << "] "
                          << "Pinned to core " << core_id
                          << " | Tile rows " << tiles[i].start_row
                          << " → " << tiles[i].end_row << std::endl;
            }

            // process assigned tile
            const Tile& tile = tiles[i];
            worker_func(tile.start_row, tile.end_row, tile.data);

            // signal completion and exit child
            sem_post(sem);
            _exit(0);
        }
        else if (pid > 0) { // parent
            workers.push_back(pid);

            // 🔵 Log parent scheduling decision
            std::cout << "[Parent " << getpid() << "] "
                      << "Spawned worker " << pid
                      << " for tile " << i
                      << " (rows " << tiles[i].start_row
                      << " → " << tiles[i].end_row << ")"
                      << std::endl;
        }
        else {
            perror("fork");
            // Clean up and exit the parent process if fork fails
            for (pid_t p : workers) waitpid(p, nullptr, 0);
            exit(1);
        }
    }

    // parent waits for children to post the semaphore
    for (size_t i = 0; i < worker_count; ++i) sem_wait(sem);

    // reap children
    for (pid_t pid : workers) waitpid(pid, nullptr, 0);
    workers.clear();
}

void IPCManager::pinProcessToCore(int core_id) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(core_id, &set);
    if (sched_setaffinity(0, sizeof(set), &set) != 0) {
        perror("sched_setaffinity");
    }
}

std::vector<int> IPCManager::getPCores() {
    int total = sysconf(_SC_NPROCESSORS_ONLN);
    int half = total / 2;
    std::vector<int> cores;
    for (int i = 0; i < half; i++) cores.push_back(i);
    return cores;
}

std::vector<int> IPCManager::getECores() {
    int total = sysconf(_SC_NPROCESSORS_ONLN);
    int half = total / 2;
    std::vector<int> cores;
    for (int i = half; i < total; i++) cores.push_back(i);
    return cores;
}
