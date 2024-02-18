/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** SharedMemory.hpp
*/

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <mutex>

#define MAX_PROCESS 100

#include "Process.hpp"

typedef struct Communication
{
    int process_ids[MAX_PROCESS];        // Array of process ids
    int process_count[MAX_PROCESS];      // Array of process counts
    int process_to_receive[MAX_PROCESS]; // Array of counts of messages to be received
    size_t isAlive;                      // Flag indicating whether the process is alive
    size_t displayCount;                 // Flag indicating to processes to print display
} Communication_t;

class SharedMemory
{
private:
    int _shmId, _semId;
    key_t _key;
    std::mutex _mutex;
    Communication_t *communication;

public:
    SharedMemory(std::string str = "shm_plazza");
    ~SharedMemory();
    void destroy();

    int getAttribute(std::string);
    void incAttribute(std::string);
    void decAttribute(std::string);
    void setAttribute(std::string, int);

    int getAttribute(std::string, int);
    void incAttribute(std::string, int);
    void decAttribute(std::string, int);
    void setAttribute(std::string, int, int);

    void setIsAlive(bool);
    bool isAlive();

    SharedMemory &operator<<(bool);
    SharedMemory &operator>>(bool &);
};

void semLock(int semId);
void semUnlock(int semId);