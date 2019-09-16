#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "directory/game_dir.hpp"
#include "creature/monster.hpp"

Monster::Monster()
{
}

void Monster::random_spawn()
{
    // TODO Need checking for an empty monster list
    // get vector of monsters from read_directory
    std::vector<std::string> monsters = read_directory(".rpg/creatures");
    // get length of vector :: vector.size()
    // generate random number in vector boundries
    int random = rand() % monsters.size();
    // call load on selected element
    load(".rpg/creatures/"+monsters[random]);
}
