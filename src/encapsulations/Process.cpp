/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Process.cpp
*/

#include "Process.hpp"

Process::Process()
{
    this->_pid = fork();
    if (this->_pid < 0)
        throw EncapsulationError("Fork failed");
}

Process::~Process() {}

void Process::wait()
{
    waitpid(this->_pid, &this->_status, 0);
}

int Process::getPid() const { return _pid; }

bool Process::isChildProcess() const
{
    return this->_pid == 0;
}