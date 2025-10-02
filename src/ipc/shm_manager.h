//
// Created by souvik on 10/2/25.
//

#ifndef MYPROJECT_SHM_MANAGER_H
#define MYPROJECT_SHM_MANAGER_H
#include <string>


class ShmManager {
public:
    ShmManager(const std::string& name, size_t size, bool create = true);
    ~ShmManager();

    void* data();
    size_t size() const { return size_; }

    void unlink();

private:
    std::string name_;
    int fd_;
    size_t size_;
    void* ptr_;
};


#endif //MYPROJECT_SHM_MANAGER_H