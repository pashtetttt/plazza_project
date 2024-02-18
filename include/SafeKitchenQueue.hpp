/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** SafePizzaQueue.hpp
*/

#pragma once

#include <unordered_map>

#include "Pizza.hpp"
#include "SafeQueue.hpp"
#include "logger.hpp"

class SafeKitchenQueue : public SafeQueue<Pizza>
{
public:
    std::unordered_map<PizzaIngredient, int> _pantry;

    void getIngredients(Pizza pizza)
    {
        for (auto ingredient : pizza.recipe)
        {
            this->_pantry.at(ingredient)--;
        }
    }

    SafeKitchenQueue()
    {
        for (int i = PizzaIngredient::Doe; i <= PizzaIngredient::ChiefLove; i++)
        {
            PizzaIngredient ingredient = static_cast<PizzaIngredient>(i);
            this->_pantry[ingredient] = 5;
        }
    };
    ~SafeKitchenQueue() = default;

    void restock()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        for (int i = PizzaIngredient::Doe; i <= PizzaIngredient::ChiefLove; i++)
        {
            PizzaIngredient ingredient = static_cast<PizzaIngredient>(i);
            this->_pantry[ingredient]++;
        }
    }

    size_t size()
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        return this->queue_.size();
    }

    void push(Pizza value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        this->cv_.notify_one();
    }

    bool tryPop(Pizza &value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return false;
        }
        value = queue_.front();
        queue_.pop();
        return true;
    }

    bool pop(Pizza &value, std::chrono::milliseconds timeout)
    {
        Pizza tmp;
        std::unique_lock<std::mutex> lock(mutex_);
        if (cv_.wait_for(lock, timeout, [this]
                         { return !queue_.empty(); }))
        {
            for (auto ingredient : queue_.front().recipe)
            {
                if (this->_pantry.at(ingredient) == 0)
                    return false;
                else if (this->_pantry.at(ingredient) < 0)
                    throw std::runtime_error("pantry shouldnt have negative ingredients");
            }
            value = queue_.front();
            this->getIngredients(value);
            queue_.pop();
            return true;
        }
        return false;
    }
};
