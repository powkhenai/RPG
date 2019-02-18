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

Creature::Creature(int strength, int agility, int intelligence, int charisma, int fortitude, int wisdom)
{
	species = "Elf";
	hp = str * fort;
	str = strength;
	agi = agility;
	intel = intelligence;
	cha = charisma;
	fort = fortitude;
	wis = wisdom;
}

std::string Creature::get_species()
{
    return species;
}

void set_str(int str)
{
	str = str;
}

void set_agi(int agi)
{
	agi=agi;
}

void set_intel(int intel)
{
	intel = intel;
}

void set_cha(int cha)
{
	cha = cha;
}

void set_fort(int fort)
{
	fort = fort;
}

void set_wis(int wis)
{
	wis = wis;
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
    fout << species << std::endl;
    fout << hp << std::endl;
    fout << str << std::endl;
    fout << agi << std::endl;
    fout << intel << std::endl;
    fout << cha << std::endl;
    fout << fort << std::endl;
    fout << wis << std::endl;
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
