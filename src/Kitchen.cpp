/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Kitchen.cpp
*/

#include <cmath>
#include <chrono>
#include <iostream>
#include <algorithm>

#include "Kitchen.hpp"
#include "logger.hpp"
#include "Reception.hpp"

void cook(SafeKitchenQueue &queue, const std::function<void()> &consumeQueue, int num, std::atomic<bool> &isKitchenOpen, std::atomic<bool> &cookIsBusy)
{
    Pizza value;

    logger::logger("Cook ", num, "\t\tallocated\n");
    while (true)
    {
        if (queue.pop(value, std::chrono::milliseconds(100)))
        {
            cookIsBusy = true;
            logger::logger("Cook ", num, "\t\treceived ", value, "\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(value.bakingTime * Reception::c));
            logger::logger("Cook ", num, "\t\tfinished with ", value, "\n");
            consumeQueue();
            cookIsBusy = false;
        }
        else if (isKitchenOpen == false)
            break;
    }
    logger::logger("Cook ", num, "\t\tclosing\n");
}

void stock(SafeKitchenQueue &queue, std::atomic<bool> &isKitchenOpen)
{
    logger::logger("Restocking\tonline\n");
    while (true)
    {
        queue.restock();
        if (!isKitchenOpen)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(Reception::r));
    }
    logger::logger("Restocking\toffline\n");
}

Kitchen::Kitchen(size_t id, size_t staffSize, const std::function<void()> &consumer) : _staffSize(staffSize), _cooksStatus(staffSize)
{
    this->id = id;
    this->_isOpen = true;

    this->_restocking = std::thread(stock, std::ref(this->_queue), std::ref(this->_isOpen));
    for (size_t i = 0; i < this->_staffSize; i++)
    {
        this->_cooksStatus[i] = false;

        this->_pool.add(
            cook, std::ref(this->_queue), consumer,
            (this->id * std::pow(10, std::floor(std::log10(staffSize)) + 1)) + i + 1, // make the cook id be equal xxyy where [KitchenId][CookId]
            std::ref(this->_isOpen), std::ref(this->_cooksStatus[i]));
    }
}

Kitchen::~Kitchen() {}

bool Kitchen::addRequest(Pizza newRequest)
{
    if (!this->isFullCapacity())
    {
        this->_queue.push(newRequest);
        return true;
    }
    logger::logger("Kitchen ", id, ": couldn't add ", newRequest, "\n");
    throw std::runtime_error("Undefinied behaviour. Shouldn't be adding request to full kitchen!");
    return false;
}

bool Kitchen::isFullCapacity()
{
    size_t pizzasWaiting = this->_queue.size();
    size_t busyCooks = this->getCooksWorking();
    return (pizzasWaiting + busyCooks) >= 2 * this->_staffSize;
}

void Kitchen::closeKitchen()
{
    this->_isOpen = false;
    this->_pool.release();
    this->_restocking.join();
}

size_t Kitchen::getCooksWorking()
{
    return std::count_if(_cooksStatus.begin(), _cooksStatus.end(), [](const std::atomic<bool> &element)
                         { return element.load(); });
}

void Kitchen::display()
{
    std::stringstream out;
    out << "Kitchen " << this->id << ", busy cooks: " << this->getCooksWorking() << std::endl;
    for (auto &it : this->_queue._pantry)
    {
        out << "\t" << it.first << ": " << it.second << std::endl;
    }
    std::cout << out.str();
}
