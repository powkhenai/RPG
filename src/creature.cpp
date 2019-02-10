#include <string>
#include <fstream>
#include <iostream>
#include "creature.hpp"

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

std::string Creature::get_species()
{
    return species;
}

int Creature::get_hp()
{
    return hp;
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

void Creature::load(const std::string file_name)
{
    std::cout << "Loaing creature from file: " << file_name << std::endl;
    std::ifstream fin;
    fin.open(file_name);
    // Read character from file
    getline(fin, species);
    std::cout << species << std::endl;
    fin >> hp;
    fin >> str;
    fin >> agi;
    fin >> intel;
    fin >> cha;
    fin >> fort;
    fin >> wis;
    fin.close();
}
