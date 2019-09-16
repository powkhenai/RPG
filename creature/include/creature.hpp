#ifndef CREATURE_H_
#define CREATURE_H_

#include <string>

class Creature
{
    private:
    std::string species;
    int hp;
    int str;
    int agi;
    int intel;
    int cha;
    int fort;
    int wis;
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
    int attack(Creature opponent);
    int defend();
    void wound(int damage);
    void heal();
    void save();
    void save(std::ofstream &fout);
    void load(std::string file_name);
    void load(std::ifstream &fin);
};
#endif
