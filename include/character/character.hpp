#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <string>
#include <unordered_map>
#include <vector>
#include "creature/creature.hpp"
#include "item/item.hpp"

class Character : public Creature
{
    private:
        std::string f_name;
        std::string l_name;
        long exp;
        std::string location;
    public:
        Character();
        Character(std::string filename);
        Character(std::string first, std::string last);
        Character(std::string first, std::string last, int strength, int agility, int intelligence, int charisma, int fortitude, int wisdom);
        std::string get_name();
        std::string get_class();
        int get_exp();
        void set_location(const std::string &new_location);
        std::string get_location();
        void award_exp(int points);
        bool consume_exp(int points);
        std::string save_file();
        void save();
        void load(std::string file_name);
        void remove();
};
#endif
