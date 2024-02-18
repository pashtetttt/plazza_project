/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Reception.cpp
*/

#include <iostream>
#include <limits>
#include <algorithm>
#include <atomic>

#include "Reception.hpp"
#include "Kitchen.hpp"
#include "logger.hpp"
#include "utils.hpp"

size_t Reception::c = 0;
size_t Reception::n = 0;
size_t Reception::r = 0;

namespace sc = std::chrono;

Reception::Reception(int argc, char const *argv[]) : _processId(1), _processCount(1)
{
    if (argc == 2 && std::string(argv[1]) == "-h")
        this->usage(0);
    if (argc != 4)
        this->usage(84);
    try
    {
        c = std::stoi(argv[1]);
        n = std::stoi(argv[2]);
        r = std::stoi(argv[3]);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << '\n';
        exit(84);
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << e.what() << '\n';
        exit(84);
    }
    this->_shared << true; // setting alive to true
    this->_shared.setAttribute("display", 0);
    this->prompt();
}

void Reception::_dispatchToKitchen(Kitchen &kitchen, int slot)
{
    std::string received;
    this->_mqueue >> received;
    if (received.length() > 0)
    {
        Order order(received);
        for (auto &pizza : order.pizzas)
            kitchen.addRequest(pizza);
        this->_shared.decAttribute("receive", slot);
    }
}

void Reception::_kitchenProcess(int slot)
{
    size_t displayCount = 0, displayGlobal;
    sc::system_clock::time_point shutdown = sc::system_clock::now() + sc::seconds(5);
    logger::logger("Kitchen ", this->_processId, "\tallocated\n");
    Kitchen kitchen(this->_processId, Reception::n, [this, slot]()
                    { this->_shared.decAttribute("count", slot); });

    while (true)
    {
        if (this->_shared.isAlive() == false || sc::system_clock::now() >= shutdown)
        {
            kitchen.closeKitchen();
            break;
        }
        displayGlobal = this->_shared.getAttribute("display");
        if (displayGlobal > displayCount)
        {
            kitchen.display();
            displayCount = displayGlobal;
        }
        if (this->_shared.getAttribute("receive", slot) > 0)
            this->_dispatchToKitchen(kitchen, slot);
        if (kitchen.getCooksWorking() > 0)
            shutdown = sc::system_clock::now() + sc::seconds(5);
    }
    this->_shared.setAttribute("id", slot, -1);
    logger::logger("Kitchen ", this->_processId, "\tclosing\n");
    exit(0);
}

int Reception::_getProcessSlot()
{
    int slot = -1;
    for (int i = 0; i < MAX_PROCESS && slot == -1; i++)
    {
        if (this->_shared.getAttribute("id", i) == -1)
            slot = i;
    }
    if (slot == -1)
    {
        this->_terminateChildren();
        throw std::runtime_error("No more slots available for new process");
    }
    return slot;
}

bool Reception::_newChild()
{
    int slot = this->_getProcessSlot();

    this->_processCount++;
    this->_shared.setAttribute("id", slot, this->_processCount);
    Process process;
    if (process.isChildProcess())
    {
        this->_processId = this->_processCount;
        this->_mqueue.id = this->_processId;
        this->_kitchenProcess(slot);
        return false;
    }
    else
        this->_children.push_back(process);
    return true;
}

Reception::~Reception() {}

void Reception::usage(int status) const
{
    std::cout << "USAGE\n./plazza c n r\n\n";
    std::cout << "    c\tmultiplier for the cooking time of the pizzas\n";
    std::cout << "    n\tnumber of cooks per kitchen\n";
    std::cout << "    r\ttime in milliseconds used by the kitchen stock to replace ingredients\n";
    exit(status);
}

int Reception::_findBestKitchen()
{
    int bestKitchen = -1;
    for (size_t i = 0, min = std::numeric_limits<size_t>::max(); i < MAX_PROCESS; i++)
    {
        int kitchenId = this->_shared.getAttribute("id", i);
        size_t kitchenCount = this->_shared.getAttribute("count", i);
        if (kitchenCount < this->n * 2 && min > kitchenCount && kitchenId != -1)
        {
            min = kitchenCount;
            bestKitchen = i;
        }
    }
    return bestKitchen;
}

void Reception::_addRequest(Pizza value)
{
    int bestKitchen = this->_findBestKitchen();

    if (bestKitchen == -1)
    {
        if (!this->_newChild())
            return;
        this->_addRequest(value);
    }
    else
    {
        this->_shared.incAttribute("count", bestKitchen);
        this->_mqueue.sendMessage(value.toString(), this->_shared.getAttribute("id", bestKitchen));
        this->_shared.incAttribute("receive", bestKitchen);
    }
}

size_t Reception::_getKitchensWorking()
{
    int count = 0;
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (this->_shared.getAttribute("id", i) != -1)
            count++;
    }
    return count;
}

void Reception::display()
{
    this->_shared.incAttribute("display");
}

void Reception::_terminateChildren()
{
    logger::logger("Reception\tstarting to close\n");
    this->_shared << false; // setting alive to false
    for (auto &process : this->_children)
        process.wait();
    logger::logger("Reception\tclosing\n");
}

void Reception::_parseOrders(std::string unparsedOrders)
{
    std::vector<Order> orders;
    try
    {
        auto tmp = utils::splitString(unparsedOrders, ";");
        for (auto &unparsedOrder : tmp)
        {
            if (unparsedOrder.length() > 0)
                orders.emplace_back(unparsedOrder);
        }
    }
    catch (const PizzaError &e)
    {
        this->_terminateChildren();
        throw e;
    }

    for (auto &order : orders)
    {
        for (auto &pizza : order.pizzas)
            this->_addRequest(pizza);
    }
}

void Reception::prompt()
{
    std::string in;
    while (this->_processId == 1)
    {
        std::cout << "> ";
        if (!getline(std::cin, in) || in == "quit" || in == "q")
        {
            this->_terminateChildren();
            break;
        }
        else if (in == "status")
            this->display();
        else
            this->_parseOrders(in);
    }
    if (this->_processId == 1)
    {
        this->_shared.destroy();
        this->_mqueue.destroy();
    }
}
