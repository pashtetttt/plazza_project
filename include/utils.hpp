/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** utils.hpp
*/

#pragma once

#include <cstring>
#include <vector>
#include <string>

namespace utils
{
    inline std::vector<std::string> splitString(const std::string &str, const std::string &delimiter)
    {
        std::vector<std::string> substrings;
        size_t start = 0;
        size_t end = str.find(delimiter);

        while (end != std::string::npos)
        {
            substrings.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }

        substrings.push_back(str.substr(start));

        return substrings;
    }

    inline std::string trim(const std::string &str)
    {
        size_t start = str.find_first_not_of(" \t\r\n");
        size_t end = str.find_last_not_of(" \t\r\n");

        if (start == std::string::npos || end == std::string::npos)
            return "";

        return str.substr(start, end - start + 1);
    }

    // Serialize struct
    template <typename T>
    inline void serializeStruct(const T &obj, char *buffer)
    {
        std::memcpy(buffer, reinterpret_cast<const char *>(&obj), sizeof(obj));
    }

    // Unserialize struct
    template <typename T>
    inline void unserializeStruct(const char *buffer, T &obj)
    {
        std::memcpy(reinterpret_cast<char *>(&obj), buffer, sizeof(obj));
    }
} // namespace utils
