#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
#include "character.hpp"
#include "game_dir.hpp"
#include "monster.hpp"

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
    characters = read_directory(".rpg");
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
    char input = '-';
    Character character;

    //Check for data dir, create it if it doesn't exist
    if(mkdir(".rpg", 0777) == -1 && strcmp(strerror(errno),"File exists") != 0)
	std::cerr << "Error: " << strerror(errno) << std::endl;
    if(mkdir(".rpg/creatures", 0777) == -1 && strcmp(strerror(errno), "File exists") != 0)
	std::cerr << "Error: " << strerror(errno) << std::endl;

    // Call the Character select menu
    character = character_select();
    std::cout << "You've chosen to play as:" << std::endl;
    std::cout << character.get_name() << std::endl;
    std::cout << character.get_species() << std::endl;
    std::cout << "Exp: " << character.get_exp() << std::endl;
    std::cout << "HP: " << character.get_hp() << std::endl;
    
    // Main game loop
    while(input != 'Q')
    {
	// Action Menu
	std::cout << "g: 'Grow your Power!'" << std::endl;
	std::cout << "f: 'Fight a Monster in the arena!'" << std::endl;
	std::cout << "Q: 'Quit Adventuring.'" << std::endl;
	std::cout << "Selection: ";
	std::cin >> input;
	// do some stuff
	if(input == 'g')
	{
	    int points = 80;
	    std::cout << character.get_name() << " has earned " << points << " experience points!" << std::endl;
	    character.award_exp(points);
	}
	if(input == 'f')
	{
	    int damage;
	    Monster monster = Monster();
	    monster.random_spawn();
	    std::cout << "A " << monster.get_species() << " walks out of the arena..." << std::endl;
	    damage = character.attack(monster);
	    if(damage > 0)
	    {
		monster.wound(damage);
	    }
	    std::cout << "The " << monster.get_species() << " hp is now: " << monster.get_hp() << std::endl;
	}
    }
    // We got "Q" so let's save the character and quit the game.
    std::cout << "Recording " << character.get_name() << "'s achievements in the Balad of the Legends!" << std::endl;
    character.save();
    return 0;
}
