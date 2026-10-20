#include <string>
#include <fstream>
#include <iostream>
#include "character/character.hpp"
#include "toml.hpp"

Character::Character()
{
    f_name = "New";
    l_name = "Guy";
    exp = 0;
}

Character::Character(std::string filename)
{
    load(filename);
}

Character::Character(std::string first, std::string last) : Creature::Creature()
{
    f_name = first;
    l_name = last;
    exp = 0;
}

Character::Character(std::string first, std::string last, int strength, int agility, int intelligence, int charisma, int fortitude, int wisdom) : Creature::Creature(strength, agility, intelligence, charisma, fortitude, wisdom)
{
    f_name = first;
    l_name = last;
    exp = 0;
}

std::string Character::get_name()
{
    return f_name+" "+l_name;
}

int Character::get_exp()
{
    return exp;
}

void Character::set_location(const std::string &new_location)
{
    location = new_location;
}

std::string Character::get_location()
{
    return location;
}

void Character::award_exp(int points)
{
    exp += points;
}

bool Character::consume_exp(int points)
{
    if (points <= exp)
    {
        exp -= points;
        return true;
    }
    return false;
}

std::string Character::save_file()
{
    return ".rpg/creatures/characters/"+f_name+"_"+l_name;
}

void Character::save()
{
    // Open a file
    std::ofstream fout;
    fout.open(save_file());
    // Serialize Character to file
    const toml::value root(toml::table{
        {"FirstName", f_name},
        {"LastName", l_name},
        {"EXP", exp},
        {"Location", location}
    });
    fout << root;
    Creature::save(fout);
    fout.close();
}

void Character::load(const std::string file_name)
{
    //std::cout << "Loading character from file: " << file_name << std::endl;
    toml::value c_file = toml::parse(file_name);
    f_name = toml::find<std::string>(c_file, "FirstName");
    l_name = toml::find<std::string>(c_file, "LastName");
    //std::cout << f_name << " " << l_name << std::endl;
    exp = toml::find<int>(c_file, "EXP");
    if (c_file.contains("Location"))
        location = toml::find<std::string>(c_file, "Location");
    else
        location = "Start";
    this->Creature::load(file_name);
    max_hp = get_str() * get_fort();
}

void Character::remove()
{
    if (std::remove(save_file().c_str()) != 0)
        std::perror("Error deleting file");
    else
        std::cout << "Deleted " << save_file() << std::endl;
}
