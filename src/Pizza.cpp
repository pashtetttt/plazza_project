/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Pizza.cpp
*/

#include <iostream>

#include "Pizza.hpp"
#include "utils.hpp"
#include "logger.hpp"

Pizza::Pizza(PizzaSize _size, PizzaType _type) : size(_size), type(_type), bakingTime(0)
{
    this->initRecipe(type);
};

Order::Order(std::string parsestr)
{

    int number;
    PizzaType type;
    PizzaSize size;

    parsestr = utils::trim(parsestr);
    auto vec = utils::splitString(parsestr, " ");

    if (vec.size() != 3)
        throw PizzaError("Bad Order: Order should have 3 parts [TYPE SIZE NUMBER]");

    std::string strType = vec[0];
    std::string strSize = vec[1];
    std::string strNumber = vec[2];

    if (!isValidType(strType, type))
        throw PizzaError("Bad Pizza: type " + strType + " specified is invalid");

    if (!isValidSize(strSize, size))
        throw PizzaError("Bad Pizza: size " + strSize + " specified is invalid");

    if (strNumber[0] != 'x')
        throw PizzaError("Bad Order: amount " + strNumber + " must start with 'x'");

    strNumber = strNumber.substr(1);
    try
    {
        number = std::stoi(strNumber);
    }
    catch (const std::exception &e)
    {
        throw PizzaError("Bad Order: number " + strNumber + " is not an int");
    }
    if (number <= 0)
        throw PizzaError("Bad Order: number " + strNumber + " has to be greater than 0");

    for (int i = 0; i < number; i++)
        this->pizzas.emplace_back(size, type);
}

bool Order::isValidType(std::string strType, PizzaType &type)
{
    type = PizzaType::BadType;
    if (strType == "Regina")
        type = PizzaType::Regina;
    if (strType == "Margarita")
        type = PizzaType::Margarita;
    if (strType == "Americana")
        type = PizzaType::Americana;
    if (strType == "Fantasia")
        type = PizzaType::Fantasia;
    return type != PizzaType::BadType;
}

bool Order::isValidSize(std::string strSize, PizzaSize &size)
{
    size = PizzaSize::BadSize;
    if (strSize == "S")
        size = PizzaSize::S;
    if (strSize == "M")
        size = PizzaSize::M;
    if (strSize == "L")
        size = PizzaSize::L;
    if (strSize == "XL")
        size = PizzaSize::XL;
    if (strSize == "XXL")
        size = PizzaSize::XXL;
    return size != PizzaSize::BadSize;
}

void Pizza::initRecipe(PizzaType _type)
{
    switch (_type)
    {
    case PizzaType::Margarita:
        bakingTime = 1000;
        recipe.push_back(PizzaIngredient::Doe);
        recipe.push_back(PizzaIngredient::Tomato);
        recipe.push_back(PizzaIngredient::Gruyere);
        break;
    case PizzaType::Regina:
        bakingTime = 2000;
        recipe.push_back(PizzaIngredient::Doe);
        recipe.push_back(PizzaIngredient::Tomato);
        recipe.push_back(PizzaIngredient::Gruyere);
        recipe.push_back(PizzaIngredient::Ham);
        recipe.push_back(PizzaIngredient::Mushrooms);
        break;
    case PizzaType::Americana:
        bakingTime = 2000;
        recipe.push_back(PizzaIngredient::Doe);
        recipe.push_back(PizzaIngredient::Tomato);
        recipe.push_back(PizzaIngredient::Gruyere);
        recipe.push_back(PizzaIngredient::Steak);
        break;
    case PizzaType::Fantasia:
        bakingTime = 4000;
        recipe.push_back(PizzaIngredient::Doe);
        recipe.push_back(PizzaIngredient::Tomato);
        recipe.push_back(PizzaIngredient::Eggplant);
        recipe.push_back(PizzaIngredient::GoatCheese);
        recipe.push_back(PizzaIngredient::ChiefLove);
        break;
    default:
        break;
    }
}

std::string Pizza::typeAsString() const
{
    if (this->type == PizzaType::Regina)
        return "Regina";
    if (this->type == PizzaType::Margarita)
        return "Margarita";
    if (this->type == PizzaType::Americana)
        return "Americana";
    if (this->type == PizzaType::Fantasia)
        return "Fantasia";
    return "";
}

std::string Pizza::sizeAsString() const
{
    if (this->size == PizzaSize::S)
        return "S";
    if (this->size == PizzaSize::M)
        return "M";
    if (this->size == PizzaSize::L)
        return "L";
    if (this->size == PizzaSize::XL)
        return "XL";
    if (this->size == PizzaSize::XXL)
        return "XXL";
    return "";
}

std::ostream &operator<<(std::ostream &os, const Pizza &obj)
{
    os << obj.typeAsString() << " " << obj.sizeAsString() << " x1";
    return os;
}
std::string Pizza::toString() const
{
    return this->typeAsString() + " " + this->sizeAsString() + " x1";
}

std::ostream &operator<<(std::ostream &os, const Order &obj)
{
    os << "Order(" << std::endl;
    for (auto &pizza : obj.pizzas)
        os << "\t" << pizza << "," << std::endl;
    os << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const PizzaIngredient &obj)
{
    switch (obj)
    {

    case PizzaIngredient::Doe:
        os << "Doe";
        break;
    case PizzaIngredient::Tomato:
        os << "Tomato";
        break;
    case PizzaIngredient::Gruyere:
        os << "Gruyere";
        break;
    case PizzaIngredient::Ham:
        os << "Ham";
        break;
    case PizzaIngredient::Mushrooms:
        os << "Mushrooms";
        break;
    case PizzaIngredient::Steak:
        os << "Steak";
        break;
    case PizzaIngredient::Eggplant:
        os << "Eggplant";
        break;
    case PizzaIngredient::GoatCheese:
        os << "GoatCheese";
        break;
    case PizzaIngredient::ChiefLove:
        os << "ChiefLove";
        break;

    default:
        break;
    }
    return os;
}
