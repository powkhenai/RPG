#include <string>
#include <fstream>
#include <iostream>
#include "character/character.hpp"
#include "cpptoml.h"

Character::Character()
{
    f_name = "New";
    l_name = "Guy";
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

void Character::save()
{
    // Open a file
    std::ofstream fout;
    std::string filename = ".rpg/"+f_name+"_"+l_name;
    fout.open(filename);
    // Serialize Character to file
    std::shared_ptr<cpptoml::table> root = cpptoml::make_table();
    root->insert("FirstName", f_name);
    root->insert("LastName", l_name);
    root->insert("EXP", exp);
    fout << *root;
    Creature::save(fout);
    fout.close();
}

void Character::load(const std::string file_name)
{
    std::cout << "Loaing character from file: " << file_name << std::endl;
    auto c_file = cpptoml::parse_file(file_name);
    f_name = *c_file->get_as<std::string>("FirstName");
    l_name = *c_file->get_as<std::string>("LastName");
    std::cout << f_name << " " << l_name << std::endl;
    exp = *c_file->get_as<int>("EXP");
    this->Creature::load(file_name);
}
