/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Pizza.hpp
*/

#pragma once

#include <string>
#include <vector>

enum PizzaType
{
    BadType = -1,
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8
};

enum PizzaSize
{
    BadSize = -1,
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16
};

enum PizzaIngredient
{
    Doe = 0,
    Tomato = 1,
    Gruyere = 2,
    Ham = 3,
    Mushrooms = 4,
    Steak = 5,
    Eggplant = 6,
    GoatCheese = 7,
    ChiefLove = 8,
};

class PizzaError : public std::exception
{
protected:
    const std::string m_error;

public:
    explicit PizzaError(std::string error) : m_error(std::move(error)){};
    [[nodiscard]] const char *what() const noexcept override { return m_error.c_str(); };
};

class Pizza
{
private:
    void initRecipe(PizzaType _type);

public:
    Pizza() = default;
    Pizza(PizzaSize _size, PizzaType _type);
    ~Pizza() = default;

    PizzaSize size;
    PizzaType type;
    std::size_t bakingTime;
    std::vector<PizzaIngredient> recipe;

    std::string typeAsString() const;
    std::string sizeAsString() const;
    std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, const Pizza &obj);

class Order
{
private:
    bool isValidType(std::string, PizzaType &);
    bool isValidSize(std::string, PizzaSize &);

public:
    std::vector<Pizza> pizzas;
    Order() = default;
    Order(std::string);
    ~Order() = default;
};

std::ostream &operator<<(std::ostream &os, const Order &obj);
std::ostream &operator<<(std::ostream &os, const PizzaIngredient &obj);
