/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** logger.hpp
*/

#pragma once

#include <mutex>
#include <iostream>
#include <condition_variable>
#include <fstream>
#include <sstream>

namespace logger
{
    inline void clear()
    {
        std::ofstream file;
        file.open("log.txt", std::ios::trunc);
        file.close();
    }

    template <typename T, typename... Args>
    std::string generateString(T type, Args... args)
    {
        std::stringstream ss;
        ss << type;

        if constexpr (sizeof...(args) > 0)
        {
            ss << generateString(args...);
        }

        return ss.str();
    }

    template <typename T, typename... Args>
    void logger(T type, Args... args)
    {
        static std::mutex loggerMutex;
        static std::condition_variable loggerCV;
        static bool isWriting = false;

        std::string message = logger::generateString(type, args...);
        std::unique_lock<std::mutex> lock(loggerMutex);

        // Wait until the logger is not writing
        loggerCV.wait(lock, []()
                      { return !isWriting; });

        // Start writing
        isWriting = true;

        // Open the log file
        std::ofstream file;
        file.open("log.txt", std::ios::app);

        // Write to the log file
        file << message;
        std::cout << message;

        // Close the log file
        file.close();

        // Finished writing
        isWriting = false;

        // Notify waiting threads
        loggerCV.notify_all();
    }
}