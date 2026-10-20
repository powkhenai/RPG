#include <string>
#include <fstream>
#include <iostream>
#include "creature/creature.hpp"
#include "toml.hpp"
#include <ctime>

Creature::Creature() {
    srand(time(NULL));
    species = "Elf";
    max_hp = rand() % 20 + 10;
    hp = max_hp;
    str = rand() % 15 + 5;
    agi = rand() % 15 + 5;
    intel = rand() % 15 + 5;
    cha = rand() % 15 + 5;
    fort = rand() % 15 + 5;
    wis = rand() % 15 + 5;
}

Creature::Creature(int strength, int agility, int intelligence, int charisma, int fortitude, int wisdom) {
    species = "Elf";
    str = strength;
    agi = agility;
    intel = intelligence;
    cha = charisma;
    fort = fortitude;
    wis = wisdom;

    max_hp = str * fort;
    hp = max_hp;
}

std::string Creature::get_species() { return species; }
void Creature::set_str(int str) { this->str = str; }
void Creature::set_agi(int agi) { this->agi = agi; }
void Creature::set_intel(int intel) { this->intel = intel; }
void Creature::set_cha(int cha) { this->cha = cha; }
void Creature::set_fort(int fort) { this->fort = fort; }
void Creature::set_wis(int wis) { this->wis = wis; }
int Creature::get_hp() { return hp; }
int Creature::get_max_hp(){ return max_hp; }
int Creature::get_str()  { return str; }
int Creature::get_agi()  { return agi; }
int Creature::get_intel(){ return intel; }
int Creature::get_cha()  { return cha; }
int Creature::get_fort() { return fort; }
int Creature::get_wis()  { return wis; }
void Creature::heal_direct() { hp = max_hp; }

void Creature::get_stats() {
    std::cout << "Current Stats:" << std::endl;
    std::cout << "STR: " << this->str << std::endl;
    std::cout << "AGI: " << this->agi << std::endl;
    std::cout << "INT: " << this->intel << std::endl;
    std::cout << "CHA: " << this->cha << std::endl;
    std::cout << "FORT: " << this->fort << std::endl;
    std::cout << "WIS: " << this->wis << std::endl;
}

int Creature::defend() {
    int defend = rand() % 20 + fort;
    //std::cout << "Defense: " << defend << std::endl;
    return defend;
}

int Creature::attack(Creature opponent) {
    int weapon_damage = 0;
    if (equipment["main_hand"] != nullptr) {
        //std::cout << "Attacking with " << equipment["main_hand"]->get_name() << std::endl;
        // Try to cast to Weapon
        auto weapon_ptr = std::dynamic_pointer_cast<Weapon>(equipment["main_hand"]);
        if (weapon_ptr) {
            weapon_damage = weapon_ptr->get_damage();
        } else {
            weapon_damage = 0; // Not a weapon, no damage
        }
    }
    else {
        //std::cout << "Attacking with bare hands!" << std::endl;
    }
    int attack = rand() % 20 + str + weapon_damage;
    // This should only ever return a positive number
    int damage = attack - opponent.defend();
    if (damage < 0) damage = 0;
    return damage;
}

void Creature::wound(int damage) { hp -= damage; }

void Creature::heal() {
    int healing = max_hp - hp;
    char input = '-';
    std::cout << "Heal " << healing << " for 10 exp points?" << std::endl;
    while(input != 'n' && input != 'y') {
    std::cout << "y: Yes\n" << "n: No" << std::endl;
    std::cin >> input;
    }
    if(input == 'y') {
    hp = max_hp;
    std::cout << "You have been healed!" << std::endl;
    }
    if(input == 'n') {
    std::cout << "You have not been healed..." << std::endl;
    }
}

void Creature::equip(std::string slot, std::shared_ptr<Item> item) {
    //std::cout << "In equip function" << std::endl;
    if (equipment.find(slot) != equipment.end()) {
        // check if slot is already occupied, if so move to inventory
        if (equipment[slot] != nullptr) {
            // TODO check if inventory has space
            inventory.push_back(equipment[slot]);
        }
        //std::cout << "found slot " << slot << std::endl;
        equipment[slot] = item;
        //std::cout << "Equipped " << item->get_name() << " to " << slot << std::endl;
        inventory.erase(std::remove(inventory.begin(), inventory.end(), item), inventory.end());
    }
}

std::vector<Action> Creature::get_actions() {
    return actions;
}

void Creature::save(std::ofstream &fout) {
    const toml::value v(toml::table{
        {"Species", species},
        {"HP", hp},
        {"STR", str},
        {"AGI", agi},
        {"INT", intel},
        {"CHA", cha},
        {"FORT", fort},
        {"WIS", wis},
        {"Action", actions}
    });
    fout << v;
}

void Creature::load(const std::string file_name) {
    //std::cout << "Loaing creature from file: " << file_name << std::endl;
    toml::value c_file = toml::parse(file_name);
    species = toml::find<std::string>(c_file, "Species");
    //std::cout << species << std::endl;
    max_hp = toml::find<int>(c_file, "HP");
    hp = toml::find<int>(c_file, "HP");
    str = toml::find<int>(c_file, "STR");
    agi = toml::find<int>(c_file, "AGI");
    intel = toml::find<int>(c_file, "INT");
    cha = toml::find<int>(c_file, "CHA");
    fort = toml::find<int>(c_file, "FORT");
    wis = toml::find<int>(c_file, "WIS");
    actions = toml::find<std::vector<Action>>(c_file, "Action");
}
