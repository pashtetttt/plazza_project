/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Kitchen.hpp
*/

#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>
#include <functional>

#include "Pizza.hpp"
#include "ThreadPool.hpp"
#include "SafeCounter.hpp"
#include "SafeKitchenQueue.hpp"

class Kitchen
{
private:
    ThreadPool _pool;
    size_t _staffSize;
    std::thread _restocking;
    SafeKitchenQueue _queue;
    std::atomic<bool> _isOpen;
    std::vector<std::atomic<bool>> _cooksStatus;

public:
    size_t id;
    Kitchen(
        size_t, size_t, const std::function<void()> &consumer = []()
                { std::cout << "Hello, World!" << std::endl; });
    ~Kitchen();
    void display();
    bool addRequest(Pizza);
    void closeKitchen(void);
    bool isFullCapacity();
    size_t getCooksWorking();
};
