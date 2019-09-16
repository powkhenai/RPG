#include <string>
#include <fstream>
#include <iostream>
#include "creature/creature.hpp"
#include "cpptoml.h"

Creature::Creature()
{
    species = "Elf";
    hp = 10;
    str = 5;
    agi = 5;
    intel = 5;
    cha = 5;
    fort = 5;
    wis = 5;
}

Creature::Creature(int strength, int agility, int intelligence, int charisma, int fortitude, int wisdom)
{
    species = "Elf";
    str = strength;
    agi = agility;
    intel = intelligence;
    cha = charisma;
    fort = fortitude;
    wis = wisdom;

    hp = str * fort;
}

std::string Creature::get_species()
{
    return species;
}

void Creature::set_str(int str)
{
    this->str = str;
}

void Creature::set_agi(int agi)
{
    this->agi = agi;
}

void Creature::set_intel(int intel)
{
    this->intel = intel;
}

void Creature::set_cha(int cha)
{
    this->cha = cha;
}

void Creature::set_fort(int fort)
{
    this->fort = fort;
}

void Creature::set_wis(int wis)
{
    this->wis = wis;
}

int Creature::get_hp()
{
    return hp;
}

int Creature::defend()
{
    int defend = rand() % 20 + fort;
    std::cout << "Defense: " << defend << std::endl;
    return defend;
}

int Creature::attack(Creature opponent)
{
    int attack = rand() % 20 + str;
    std::cout << "Attack: " << attack << std::endl;
    return attack - opponent.defend();
}

void Creature::wound(int damage)
{
    hp -= damage;
}

void Creature::heal()
{
    int max_hp = str * fort;
    int healing = max_hp - hp;
    char input = '-';
    std::cout << "Heal " << healing << " for 10 exp points?" << std::endl;
    while(input != 'n' && input != 'y')
    {
    std::cout << "y: Yes\n" << "n: No" << std::endl;
    std::cin >> input;
    }
    if(input == 'y')
    {
    hp = max_hp;
    std::cout << "You have been healed!" << std::endl;
    }
    if(input == 'n')
    {
    std::cout << "You have not been healed..." << std::endl;
    }
}

void Creature::save()
{
    // Open a file
    std::ofstream fout;
    std::string filename = ".rpg/creatures/"+species;
    fout.open(filename);
    // Serialize Creature to file
    fout << species << std::endl;
    fout << hp << std::endl;
    fout << str << std::endl;
    fout << agi << std::endl;
    fout << intel << std::endl;
    fout << cha << std::endl;
    fout << fort << std::endl;
    fout << wis << std::endl;
    fout.close();
}

void Creature::save(std::ofstream &fout)
{
    // Serialize Creature to file
    std::shared_ptr<cpptoml::table> root = cpptoml::make_table();
    root->insert("Species", species);
    root->insert("HP", hp);
    root->insert("STR", str);
    root->insert("AGI", agi);
    root->insert("INT", intel);
    root->insert("CHA", cha);
    root->insert("FORT", fort);
    root->insert("WIS", wis);
    fout << *root;
}

void Creature::load(const std::string file_name)
{
    std::cout << "Loaing creature from file: " << file_name << std::endl;
    auto c_file = cpptoml::parse_file(file_name);
    species = c_file->get_as<std::string>("Species").value_or("Elf");
    std::cout << species << std::endl;
    hp = *c_file->get_as<int>("HP");
    str = *c_file->get_as<int>("STR");
    agi = *c_file->get_as<int>("AGI");
    intel = *c_file->get_as<int>("INT");
    cha = *c_file->get_as<int>("CHA");
    fort = *c_file->get_as<int>("FORT");
    wis = *c_file->get_as<int>("WIS");
}

void Creature::load(std::ifstream &fin)
{
    getline(fin, species);
    std::cout << species << std::endl;
    fin >> hp;
    fin >> str;
    fin >> agi;
    fin >> intel;
    fin >> cha;
    fin >> fort;
    fin >> wis;
}
