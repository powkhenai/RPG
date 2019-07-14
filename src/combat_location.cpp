#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "game_dir.hpp"
#include "locations/combat_location.hpp"

Combat_Location::Combat_Location()
{
    std::map<std::string, int> spawn_table;
}

void Combat_Location::load(const std::string file_name)
{
    std::cout << "Loading location from file: " << file_name << std::endl;
    // Read Combat_location from file
    //getline(fin, name);
    //getline(fin, description);
    //std::cout << name << std::endl;
    std::string item;
    this->Location::load(file_name);
    /*
    while(getline(fin, item))
    {
        spawn_table[item.substr(0, item.find(":"))] = stoi(item.substr(1, item.find(":")));
    }
    */
}
