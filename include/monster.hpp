#ifndef MONSTER_H_
#define MONSTER_H_

#include <string>
#include "creature.hpp"

class Monster : public Creature
{
    private:
	// ??
    public:
	Monster();
	int defend();
	int attack();
	void random_spawn();
};
#endif
