#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
#include "player.hpp"

void read_directory(const std::string& name, std::vector<std::string>& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) 
    {
	if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
	{
	    v.push_back(dp->d_name);
	}
    }
    closedir(dirp);
}

int main()
{
    int menu_count = 0;
    std::vector<std::string> characters;

    //Check for data dir, create it if it doesn't exist
    if (mkdir(".rpg", 0777) == -1 && strcmp(strerror(errno),"File exists") != 0)
	std::cerr << "Error: " << strerror(errno) << std::endl;

    std::cout << "Load existing character or create a new one: " << std::endl;
    std::cout << menu_count << ": New Character" << std::endl;
    read_directory(".rpg", characters);
    for (const std::string character : characters)
    {
	menu_count++;
	std::cout << menu_count << ": " << character << std::endl;
    }
    //Player bob = Player();
    //std::cout << "Shall we play a game as " << bob.get_name() << std::endl;
    //bob.save();
    return 0;
}
