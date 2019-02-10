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
	std::string get_name();
	std::string get_class();
	int get_exp();
	void award_exp(int points);
	void save();
	void load(std::string file_name);
};
#endif
