/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Process.hpp
*/

#pragma once

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>

class EncapsulationError : public std::exception
{
protected:
    const std::string m_error;

public:
    explicit EncapsulationError(std::string error) : m_error(std::move(error)){};
    [[nodiscard]] const char *what() const noexcept override { return m_error.c_str(); };
};

class Process
{
private:
    int _shmId;
    pid_t _pid;
    int _status;

public:
    Process();
    ~Process();
    void wait();
    pid_t getPid() const;
    bool isChildProcess() const;
};
