#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <string>
#include "creature.hpp"

class Character : public Creature
{
    private:
    std::string f_name;
    std::string l_name;
    long exp;
    public:
    Character();
    Character(std::string first, std::string last, int strength, int agility, int intelligence, int charisma, int fortitude, int wisdom);
    std::string get_name();
    std::string get_class();
    int get_exp();
    void award_exp(int points);
    bool consume_exp(int points);
    void save();
    void load(std::string file_name);
};
#endif
