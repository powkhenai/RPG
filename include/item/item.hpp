#pragma once

#include <string>

class Item
{
    private:
        std::string name;
        std::string type;
        float weight;
        int value;
        int durability;
        // TODO Enchantments Vector of enchantment objects
    public:
        Item();
        Item(std::string name, std::string type, int value, float weight, int durability);
        std::string get_name();
        std::string get_type();
        int get_weight();
        int get_value();
        int get_durability();
        void damage(int damage);
        void repair();
        void save();
        void save(std::ofstream &fout);
        void load(std::string file_name);
        void load(std::ifstream &fin);
        virtual ~Item() = default;
};

class Weapon : public Item
{
    private:
    int damage;
    std::string damage_type;
    public:
    Weapon();
    Weapon(std::string name, std::string type, int value, float weight, int durability, int damage, std::string damage_type);
    int get_damage();
    std::string get_damage_type();
};
