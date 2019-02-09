#include <string>
#include <fstream>
#include <iostream>
#include "character.hpp"

Character::Character()
{
    f_name = "Test";
    l_name = "Guy";
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
    fin.close();
}
