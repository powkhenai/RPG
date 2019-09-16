#ifndef ITEM_H_
#define ITEM_H_

#include <string>

class Item
{
    private:
    std::string name;
    std::string type;
    int weight;
    int value;
    // TODO Enchantments Vector of enchantment objects
    public:
    Item();
    Item(std::string name, std::string type, int weight, int value);
    std::string get_name();
    int get_weight();
    int get_value();
    int defend();
    void wound(int damage);
    void heal();
    void save();
    void save(std::ofstream &fout);
    void load(std::string file_name);
    void load(std::ifstream &fin);
};
#endif
