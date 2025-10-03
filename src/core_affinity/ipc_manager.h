//
// Created by souvik on 10/2/25.
//

#ifndef MYPROJECT_IPC_MANAGER_H
#define MYPROJECT_IPC_MANAGER_H
#include <functional>
#include <string>
#include <semaphore.h>

#include "tile_manager.h"


class IPCManager {
public:
    enum class CoreStrategy {
        P_ONLY,
        E_ONLY,
        MIXED
    };

    IPCManager(int num_workers,
               const std::string& sem_name,
               unsigned char* shm_ptr,
               CoreStrategy strategy = CoreStrategy::MIXED);

    ~IPCManager();

    // Launch workers across tiles
    void launchWorkers(const std::vector<Tile>& tiles,
                      std::function<void(int, int, unsigned char*)> worker_func);

private:
    int num_workers;
    std::string sem_name;
    unsigned char* shm_ptr;
    sem_t* sem;
    std::vector<pid_t> workers;
    CoreStrategy core_strategy;

    static void pinProcessToCore(int core_id);

    static std::vector<int> getPCores();

    static std::vector<int> getECores();
};

#endif //MYPROJECT_IPC_MANAGER_H