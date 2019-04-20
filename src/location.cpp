#include <string>
#include <fstream>
#include <iostream>
#include "locations/location.hpp"

Location::Location()
{
    name = "Arena";
    description = "The high walls and dirt floor of the Arena greet you. The smell of blood and death is inescapable.";
}

Location::Location(std::string place, std::string desc)
{
    name = place;
    description = desc;
}

std::string Location::get_name()
{
    return name;
}

std::string Location::get_description()
{
    return description;
}

void Location::load(const std::string file_name)
{
    std::cout << "Loading location from file: " << file_name << std::endl;
    std::ifstream fin;
    fin.open(file_name);
    // Read character from file
    getline(fin, name);
    getline(fin, description);
    std::cout << name << std::endl;
    fin.close();
}

void Location::load(std::ifstream &fin)
{
    getline(fin, name);
    getline(fin, description);
    std::cout << name << std::endl;
}
