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
	std::string get_species();
	int get_hp();
	int attack(Creature opponent);
	int defend();
	void wound(int damage);
	void save();
	void load(std::string file_name);
};
#endif
