#ifndef COMBAT_LOC_H_
#define COMBAT_LOC_H_

#include <string>
#include "location.hpp"

class Combat_Location : public Location
{
    private:
        std::map<Monster, float>;
        // vector of items?
        //    Use map to map spawn rates?
    public:
        Combat_location();
        void spawn_from_list();
};
#endif
