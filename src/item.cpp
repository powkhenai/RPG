#include <string>
#include <fstream>
#include <iostream>
#include "item/item.hpp"
#include "toml.hpp"

Item::Item()
{
    name = "Coin";
    type = "Trinket";
    value = 1;
    weight = 0.1;
    durability = 1;
}

Item::Item(std::string in_name, std::string in_type,  int in_value, float in_weight, int in_durability)
{
    name = in_name;
    type = in_type;
    value = in_value;
    weight = in_weight;
    durability = in_durability;
}

std::string Item::get_name()
{
    return name;
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
    const toml::value root(toml::table{
        {"Name", name},
        {"Type", type},
        {"Value", value},
        {"Weight", weight},
        {"Durability", durability}
    });
    fout << root;
}

void Item::load(const std::string file_name)
{
    std::cout << "Loading item from file: " << file_name << std::endl;
    auto c_file = toml::parse(file_name);
    name = toml::find<std::string>(c_file, "Name");
    type = toml::find<std::string>(c_file, "Type");
    value = toml::find<int>(c_file, "Value");
    weight = toml::find<double>(c_file, "Weight");
    durability = toml::find<int>(c_file, "Durability");
}

Weapon::Weapon() : Item::Item()
{
    damage = 1;
    damage_type = "Bludgeoning";
}

Weapon::Weapon(std::string name, std::string type, int value, float weight, int durability, int damage, std::string damage_type) : Item(name, type, value, weight, durability)
{
    this->damage = damage;
    this->damage_type = damage_type;
}

int Weapon::get_damage()
{
    return damage;
}

std::string Weapon::get_damage_type()
{
    return damage_type;
}
