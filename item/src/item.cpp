#include <string>
#include <fstream>
#include <iostream>
#include "item/item.hpp"
#include "cpptoml.h"

Item::Item()
{
    name = "Coin";
    type = "Trinket";
    value = 1;
    weight = 0.1;
    durability = 1;
}

Item::Item(std::string name, std::string type,  int value, float weight, int durability)
{
    name = name;
    type = type;
    value = value;
    weight = weight;
    durability = durability;
}

std::string Item::get_type()
{
    return type;
}

int Item::get_durability()
{
    return durability;
}

void Item::damage(int val)
{
    durability -= val;
}

void Item::repair()
{
   // Reset durability to max? or some percentage thereof? 
   durability = 10;
}

void Item::save(std::ofstream &fout)
{
    // Serialize Item to file
    std::shared_ptr<cpptoml::table> root = cpptoml::make_table();
    root->insert("Name", name);
    root->insert("Type", type);
    root->insert("Value", value);
    root->insert("Weight", weight);
    root->insert("Durability", durability);
    fout << *root;
}

void Item::load(const std::string file_name)
{
    std::cout << "Loaing item from file: " << file_name << std::endl;
    auto c_file = cpptoml::parse_file(file_name);
    name = *c_file->get_as<std::string>("Name");
    type = *c_file->get_as<std::string>("Type");
    value = *c_file->get_as<int>("Value");
    weight = *c_file->get_as<double>("Weight");
    durability = *c_file->get_as<int>("Durability");
}
