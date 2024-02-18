/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** ThreadPool.hpp
*/

#pragma once

#include <deque>
#include <atomic>
#include <thread>
#include <algorithm>

class ThreadPool
{
private:
    std::deque<std::thread> _threads;

public:
    ThreadPool() = default;
    ~ThreadPool()
    {
        release();
    }
    template <typename function, typename... Args>
    void add(function &&fn, Args &&...args)
    {
        _threads.emplace_back(std::forward<function>(fn), std::forward<Args>(args)...);
    }
    void release()
    {
        for (auto &thread : _threads)
            thread.join();
        _threads.clear();
    }
};
