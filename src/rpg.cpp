#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
#include "character.hpp"

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

// character select function
Character character_select()
{
    int menu_count = 0;
    int input = -1;
    std::vector<std::string> characters;
    Character loaded = Character();

    std::cout << "Load existing character or create a new one: " << std::endl;
    // can call create_new_character()
    std::cout << menu_count << ": New Character" << std::endl;
    read_directory(".rpg", characters);
    // Shows character list
    for (const std::string character : characters)
    {
	menu_count++;
	std::cout << menu_count << ": " << character << std::endl;
    }

    while(input < 0)
    {
	std::cout << "Selection: ";
	std::cin >> input;
	// Do some input sanitation / checking here
    }
    std::cout << "You've selected: " << characters[input-1] << std::endl;

    if(input != 0)
    {
	loaded.load(".rpg/"+characters[input-1]);
    }

    // returns a loaded character object
    return loaded;
}

int main()
{
    //Check for data dir, create it if it doesn't exist
    if (mkdir(".rpg", 0777) == -1 && strcmp(strerror(errno),"File exists") != 0)
	std::cerr << "Error: " << strerror(errno) << std::endl;

    // Call the Character select menu
    Character character = character_select();
    std::cout << "You've chosen to play as " << character.get_name() << std::endl;
    std::cout << "Exp: " << character.get_exp() << std::endl;
    
    //Character bob = Character();
    //std::cout << "Shall we play a game as " << bob.get_name() << std::endl;
    //bob.save();
    return 0;
}
