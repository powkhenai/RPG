#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>

class Player
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
	Player();
	std::string get_name();
	std::string get_class();
	void save();
	void load(std::string fname);
};
#endif
