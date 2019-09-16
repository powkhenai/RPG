#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <term.h>
#include "character/character.hpp"
#include "directory/game_dir.hpp"
#include "creature/monster.hpp"
#include "location/combat_location.hpp"
#include "encounter.hpp"

// Clear the screen
void ClearScreen()
{
    if(!cur_term)
    {
    int result;
    setupterm(NULL, STDOUT_FILENO, &result);
    if(result <= 0) return;
    }
    putp(tigetstr("clear"));
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

    if(input != 0)
    {
        std::cout << "You've selected: " << characters[input-1] << std::endl;
    loaded.load(".rpg/"+characters[input-1]);
    }

    if(input == 0)
    {
    std::string fname;
    std::string lname;
    int str;
    int agi;
    int intel;
    int cha;
    int fort;
    int wis;

    std::cout << "Enter First Name: ";
    std::cin >> fname;
    std::cout << "Enter Last Name: ";
    std::cin >> lname;
    std::cout << "Enter Strength Score: ";
    std::cin >> str;
    std::cout << "Enter Agility Score: ";
    std::cin >> agi;
    std::cout << "Enter Intelligence Score: ";
    std::cin >> intel;
    std::cout << "Enter Charisma Score: ";
    std::cin >> cha;
    std::cout << "Enter Fortitude Score: ";
    std::cin >> fort;
    std::cout << "Enter Wisdome Score: ";
    std::cin >> wis;
    loaded = Character(fname, lname, str, agi, intel, cha, fort, wis);
    }
    // returns a loaded character object
    return loaded;
}


int main()
{
    char input = '-';
    Character character;
    Location arena;

    //Check for data dir, create it if it doesn't exist
    if(mkdir(".rpg", 0777) == -1 && strcmp(strerror(errno),"File exists") != 0)
    std::cerr << "Error: " << strerror(errno) << std::endl;
    if(mkdir(".rpg/creatures", 0777) == -1 && strcmp(strerror(errno), "File exists") != 0)
    std::cerr << "Error: " << strerror(errno) << std::endl;

    // Call the Character select menu
    character = character_select();
    std::cout << "You've chosen to play as:" << std::endl;

    // Test location loading
    arena = Combat_Location();
    arena.load(".rpg/locations/Arena");

    // Main game loop
    while(input != 'Q')
    {
        std::cout << character.get_name() << std::endl;
        std::cout << character.get_species() << std::endl;
        std::cout << "Exp: " << character.get_exp() << std::endl;
        std::cout << "HP: " << character.get_hp() << std::endl;
        std::cout << std::endl;

        // Action Menu
        std::cout << "Menu:" << std::endl;
        std::cout << "g: 'Grow your Power!'" << std::endl;
        std::cout << "f: 'Fight a Monster in the arena!'" << std::endl;
        std::cout << "h: 'Heal up and replentish HP!'" << std::endl;
        std::cout << "Q: 'Quit Adventuring.'" << std::endl;
        std::cout << "Selection: ";
        std::cin >> input;
        // Power Up Menu
        if(input == 'g')
        {
            // Offer options here to buy stat points or levels for EXP
            int points = 80;
            std::cout << character.get_name() << " has earned " << points << " experience points!" << std::endl;
            character.award_exp(points);
        }
        // Heal Menu
        if(input == 'h')
        {
            // Currently full heal for 10 EXP, maybe something more complex/interesting?
            if (character.consume_exp(10))
            {
                character.heal();
            }
        }
        // Combat menu
        if(input == 'f')
        {
            // Currently runs a combat loop in the arena, may replace this with an "Exploration menu" and move combat into it's own Encounter? class?
            int result;
            int damage;
            Monster monster = Monster();
            monster.random_spawn();
            Encounter encounter = Encounter(character, monster);
            std::cout << "A " << monster.get_species() << " walks out of the arena..." << std::endl;
            encounter.main_menu();
            result = encounter.get_result();
            if(result == 1)
            {
            std::cout << "You were victorious!" << std::endl;
            std::cout << "Awarding 5xp." << std::endl;
            character.award_exp(5);
            }
            if(result == 0)
            {
            std::cout << "You have fallen in combat..." << std::endl;
            std::cout << "You're legacy ends here. Perhaps future generations will carry on your legacy." << std::endl;
            }
            if(result == -1)
            {
            std::cout << "You managed to escape the " << monster.get_species() << " but you didn't get any stronger from the encounter." << std::endl;
            }
            // double cin.get() to wait for an enter press before clearing the screen.
            std::cin.get();
            std::cin.get();
            ClearScreen();
        }
    }
    // We got "Q" so let's save the character and quit the game.
    std::cout << "Recording " << character.get_name() << "'s achievements in the Balad of the Legends!" << std::endl;
    character.save();
    return 0;
}
