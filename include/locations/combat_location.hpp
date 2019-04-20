#ifndef COMBAT_LOC_H_
#define COMBAT_LOC_H_

#include <string>
#include <map>
#include "location.hpp"
#include "creatures/monster.hpp"

class Combat_Location : public Location
{
    protected:
        std::map<std::string, int> spawn_table;
        // vector of items?
        //    Use map to map spawn rates?
    public:
        Combat_Location();
        void load(const std::string filename);
        void spawn_from_list();
};
#endif
