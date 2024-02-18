/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Reception.hpp
*/

#pragma once

#include <thread>
#include <atomic>

#include "Pizza.hpp"
#include "SafeQueue.hpp"
#include "SafeCounter.hpp"
#include "Kitchen.hpp"
#include "Process.hpp"
#include "SharedMemory.hpp"
#include "MessageQueue.hpp"

class Reception
{
private:
    size_t _processId;
    size_t _processCount;
    SharedMemory _shared;
    MessageQueue _mqueue;
    std::vector<Process> _children;

    int _getProcessSlot();
    int _findBestKitchen();
    bool _newChild();
    void _addRequest(Pizza);
    void _terminateChildren();
    void _parseOrders(std::string);
    size_t _getKitchensWorking();

    void _kitchenProcess(int);
    void _dispatchToKitchen(Kitchen&, int);

public:
    static size_t c, n, r;
    Reception(int, char const *[]);
    ~Reception();
    void usage(int) const;
    void prompt();
    void display();
};

void kitchen_thread(SafeQueue<Pizza> &, SafeCounter &, std::atomic<bool> &, std::atomic<bool> &, std::atomic<size_t> &);
