#ifndef LOCATION_H_
#define LOCATION_H_

#include <string>

class Location
{
    private:
        std::string name;
        std::string description;
        bool hostile;
        // These probably all belong in a subclass
        // vector of monsters?
        // vector of items?
        //    Use map to map spawn rates?
    public:
        Location();
        Location(std::string name, std::string description, bool hostile);
        std::string get_name();
        std::string get_description();
        bool is_hostile();
        void save();
        void save(std::ofstream &fout);
        void load(std::string file_name);
        void load(std::ifstream &fin);
};
#endif
