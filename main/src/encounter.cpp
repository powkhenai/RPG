#include <string>
#include <fstream>
#include <iostream>
#include "encounter.hpp"
#include "character/character.hpp"
#include "creature/monster.hpp"

Encounter::Encounter()
{
    player = Character();
    opponent = Monster();
    result = 10;
}

Encounter::Encounter(Character &play, Monster &opp)
{
    player = play;
    opponent = opp;
    result = 10;
}

void Encounter::main_menu()
{
    char input = '-';
    int damage;
    std::cout << "f: Fight!\n"
              << "s: Swindle\n"
              << "b: Bribe\n"
              << "r: Run"
              << std::endl;
    std::cout << "Selection: ";
    std::cin >> input;
    if(input == 'f')
    {
        // Fight
        combat();
    }
    else
    {
        main_menu();
    }
}

void Encounter::combat()
{
    std::cout << "Entering Combat" << std::endl;
    while(player.get_hp() > 0 && opponent.get_hp() > 0 && result != -1)
    {
        char input = '-';
        int damage;
        std::cout << "a: Attack!\n" << "e: Escape!" << std::endl;
        std::cout << "Selection: ";
        std::cin >> input;
        if(input == 'a')
        {
            // Attack
            damage = player.attack(opponent);
            if(damage > 0)
            {
            opponent.wound(damage);
                std::cout << "You hit the " << opponent.get_species() << " for " << damage << " points." << std::endl;
            }
            else
            std::cout << "With a mighty swing... you completely miss the " << opponent.get_species() << "." << std::endl;
            if(opponent.get_hp() <= 0)
            {
            result = 1; // The player won the combat
            break;
            }
            damage = opponent.attack(player);
            if(damage > 0)
            {
            player.wound(damage);
                std::cout << "The " << opponent.get_species() << " hits you for " << damage << " points." << std::endl;
            }
            else
            std::cout << "You depftly dodge the " << opponent.get_species() << "'s attack!." << std::endl;
            if(player.get_hp() <= 0)
            {
            result = 0; // The Opponent won the combat
            break;
            }
        }
        std::cout << player.get_name() << "'s hp is: " << player.get_hp() << std::endl;
        std::cout << opponent.get_species() << "'s hp is: " << opponent.get_hp() << std::endl;
        if(input == 'e')
        {
            result = -1; // The player fled combat
        }
    }
}

int Encounter::get_result()
{
    return result;
}
