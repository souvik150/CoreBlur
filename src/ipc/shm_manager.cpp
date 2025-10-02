//
// Created by souvik on 10/2/25.
//

#include "shm_manager.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

ShmManager::ShmManager(const std::string& name, size_t size, bool create)
    : name_(name), fd_(-1), size_(size), ptr_(nullptr)
{
    int flags = create ? (O_CREAT | O_RDWR) : O_RDWR;
    fd_ = shm_open(name_.c_str(), flags, 0666);
    if (fd_ == -1) throw std::runtime_error("shm_open failed");

    if (create) {
        if (ftruncate(fd_, static_cast<off_t>(size_)) == -1) {
            shm_unlink(name_.c_str());
            throw std::runtime_error("ftruncate failed");
        }
    }

    ptr_ = mmap(nullptr, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if (ptr_ == MAP_FAILED) {
        shm_unlink(name_.c_str());
        throw std::runtime_error("mmap failed");
    }
}

ShmManager::~ShmManager()
{
    if (ptr_) munmap(ptr_, size_);
    if (fd_ != -1) close(fd_);
}

void* ShmManager::data() { return ptr_; }

void ShmManager::unlink() { shm_unlink(name_.c_str()); }
