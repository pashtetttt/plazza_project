/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** SafeCounter.hpp
*/

#pragma once

#include <mutex>

class SafeCounter
{
private:
    size_t _counter;
    std::mutex _mutex;

public:
    SafeCounter() : _counter(0) {}
    SafeCounter(size_t counter) : _counter(counter) {}

    ~SafeCounter(){};

    void inc() {
        std::unique_lock<std::mutex> lock(_mutex);
        this->_counter++;
    }

    void dec() {
        std::unique_lock<std::mutex> lock(_mutex);
        this->_counter--;
    }

    size_t get() {
        std::unique_lock<std::mutex> lock(_mutex);
        return this->_counter;
    }

    void set(size_t val) {
        std::unique_lock<std::mutex> lock(_mutex);
        this->_counter = val;
    }
};
