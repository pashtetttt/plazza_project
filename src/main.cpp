/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** main.cpp
*/

#include <iostream>
#include <memory>

#include "logger.hpp"
#include "Reception.hpp"

int main(int argc, char const *argv[])
{
    try
    {
        logger::clear();
        Reception app(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 84;
    }
    return 0;
}
