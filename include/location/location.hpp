#pragma once

#include <string>
#include <vector>
#include "actions/action.hpp"

class Location
{
    protected:
        std::string name;
        std::string description;
        std::vector<Action> actions;
        bool hostile;
        // These probably all belong in a subclass
        // vector of monsters?
        // vector of items?
        //    Use map to map spawn rates?
    public:
        Location();
        Location(std::string name, std::string description);
        std::string get_name();
        std::string get_description();
        std::vector<Action> get_actions();
        bool is_hostile();
        void save();
        void save(std::ofstream &fout);
        void load(std::string file_name);
        void load(std::ifstream &fin);
};
