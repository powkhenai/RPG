#ifndef ENCOUNTER_H_
#define ENCOUNTER_H_

#include <string>
#include "creature/monster.hpp"
#include "character/character.hpp"

class Encounter
{
    protected:
        Character player;
        Monster opponent;
        int result;
    public:
        Encounter();
        Encounter(Character &player, Monster &opponent);
        void main_menu();
        int get_result();
        void combat();
        void swindle();
        void bribe();
        void run();
};
#endif
