#include <string>
#include <fstream>
#include <iostream>
//#include <filesystem>
#include "toml.hpp"
#include "location/location.hpp"

Location::Location()
{
    name = "The Void";
    description = "An empty, featureless expanse. You shouldn't be here.";
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

std::vector<Action> Location::get_actions()
{
    return actions;
}

void Location::load(const std::string file_name)
{
    //std::cout << "Loading location from file: " << file_name << std::endl;
    if (true) //std::filesystem::exists(file_name))
    {
        auto l_file = toml::parse(file_name);
        // Read location from file
        name = toml::find<std::string>(l_file, "name");
        description = toml::find<std::string>(l_file, "description");
        actions = toml::find<std::vector<Action>>(l_file, "Action");
        //std::cout << name << std::endl;
        //std::cout << description << std::endl;
    }
    else
    {
        std::cout << file_name << " does not exist, the data directory seems to be corrupt." << std::endl;
    }
}

void Location::load(std::ifstream &fin)
{
    getline(fin, name);
    getline(fin, description);
    //std::cout << name << std::endl;
}
