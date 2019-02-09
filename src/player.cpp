#include <string>
#include <fstream>
#include "player.hpp"

Player::Player()
{
    f_name = "Test";
    l_name = "Guy";
    exp = 0;
}

std::string Player::get_name()
{
    return f_name+" "+l_name;
}

void Player::save()
{
    // Open a file
    std::ofstream fout;
    std::string filename = ".rpg/"+f_name+"_"+l_name+".ply";
    fout.open(filename);
    // Serialize player to file
    fout << f_name << std::endl;
    fout << l_name << std::endl;
    fout << exp << std::endl;
    fout.close();
}
