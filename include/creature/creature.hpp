#ifndef CREATURE_H_
#define CREATURE_H_

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "item/item.hpp"
#include "actions/action.hpp"

class Creature
{
    protected:
    std::string species;
    int hp;
    int max_hp;
    int str;
    int agi;
    int intel;
    int cha;
    int fort;
    int wis;

    std::unordered_map <std::string, std::shared_ptr<Item>> equipment = {
        {"head", nullptr},
        {"neck", nullptr},
        {"body", nullptr},
        {"ring1", nullptr},
        {"ring2", nullptr},
        {"feet", nullptr},
        {"main_hand", nullptr},
        {"off_hand", nullptr}
    };
    std::vector <std::shared_ptr<Item>> inventory = {};
    std::vector<Action> actions;

    public:
    Creature();
    Creature(int strength, int agility, int intelligence, int charisma, int fortitude, int wisdom);
    std::string get_species();
    void set_str(int str);
    void set_agi(int agi);
    void set_intel(int intel);
    void set_cha(int cha);
    void set_fort(int fort);
    void set_wis(int wis);
    int get_hp();
    int get_max_hp();
    int get_str();
    int get_agi();
    int get_intel();
    int get_cha();
    int get_fort();
    int get_wis();
    void get_stats();
    void heal_direct();
    std::vector<Action> get_actions();
    int attack(Creature opponent);
    int defend();
    void wound(int damage);
    void heal();
    void equip(std::string slot, std::shared_ptr<Item> item);
    void save();
    void save(std::ofstream &fout);
    void load(std::string file_name);
    void load(std::ifstream &fin);
};
#endif
