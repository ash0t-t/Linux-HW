#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include <semaphore.h>

class shared_array {
private:
    int *data;
    size_t size;
    std::string name;
    sem_t *sem;

    void initialize_shared_memory(bool create) {
        int flags = O_RDWR | (create ? O_CREAT : 0);
        int fd = shm_open(name.c_str(), flags, 0666);
        if (fd == -1)
            throw std::runtime_error("failed to open shared memory");

        if (create && ftruncate(fd, size * sizeof(int)) == -1) {
            close(fd);
            throw std::runtime_error("failed to set shared memory size");
        }

        data = static_cast<int *>(mmap(nullptr, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        close(fd);
        if (data == MAP_FAILED)
            throw std::runtime_error("failed to map shared memory");
    }

    void initialize_semaphore(bool create) {
        sem = sem_open((name + "_sem").c_str(), create ? O_CREAT : 0, 0666, 1);
        if (sem == SEM_FAILED)
            throw std::runtime_error("failed to open semaphore");
    }

public:
    shared_array(const std::string &array_name, size_t array_size)
        : size(array_size), name(array_name), data(nullptr), sem(nullptr) {
        if (size < 1 || size > 1'000'000'000)
            throw std::invalid_argument("invalid array size");

        bool create = (shm_open(name.c_str(), O_RDWR, 0666) == -1);
        initialize_shared_memory(create);
        initialize_semaphore(create);
    }

    ~shared_array() {
        munmap(data, size * sizeof(int));
        sem_close(sem);
    }

    int &operator[](size_t index) {
        if (index >= size)
            throw std::out_of_range("index is out of range");
        return data[index];
    }

    void lock() { sem_wait(sem); }
    void unlock() { sem_post(sem); }
};

#endif
