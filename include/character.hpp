#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <string>

class Character
{
    private:
	std::string f_name;
	std::string l_name;
	int hp;
	long exp;
	int str;
	int agi;
	int intel;
	int cha;
	int fort;
	int wis;
    public:
	Character();
	std::string get_name();
	std::string get_class();
	int get_exp();
	void save();
	void load(std::string file_name);
};
#endif
