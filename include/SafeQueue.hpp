/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** SafeQueue.hpp
*/

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue
{
public:
    SafeQueue() = default;
    ~SafeQueue() = default;

	SafeQueue(const SafeQueue&) = delete;
	SafeQueue& operator=(const SafeQueue&) = delete;

    size_t size()
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        return this->queue_.size();
    }

    void push(T value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        this->cv_.notify_one();
    }

    bool tryPop(T &value)
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

    bool pop(T &value, std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (cv_.wait_for(lock, timeout, [this]
                         { return !queue_.empty(); }))
        {
            value = queue_.front();
            queue_.pop();
            return true;
        }
        return false;
    }

protected:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
};
