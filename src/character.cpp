#include <string>
#include <fstream>
#include <iostream>
#include "character.hpp"

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
    fout << f_name << std::endl;
    fout << l_name << std::endl;
    fout << exp << std::endl;
    Creature::save(fout);
    fout.close();
}

void Character::load(const std::string file_name)
{
    std::cout << "Loaing character from file: " << file_name << std::endl;
    std::ifstream fin;
    fin.open(file_name);
    // Read character from file
    getline(fin, f_name);
    getline(fin, l_name);
    std::cout << f_name << " " << l_name << std::endl;
    fin >> exp;
    // Consume newline character after reading in exp value.
    fin.ignore(256, '\n');
    this->Creature::load(fin);
    fin.close();
}
