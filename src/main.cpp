#include <algorithm>
#include <random>
#include <iterator>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include <cstdlib>
#include <sstream>
#include <unistd.h>

#include "character/character.hpp"
#include "creature/monster.hpp"
#include "location/combat_location.hpp"
#include "directory/game_dir.hpp"
#include "item/item.hpp"
#include "tui.hpp"

namespace fs = std::filesystem;

void validate_data_dir() {
    if (!fs::exists(".rpg") && fs::exists("data")) {
        fs::create_directory(".rpg");
        fs::copy("data", ".rpg", fs::copy_options::recursive);
    } else if (!fs::exists(".rpg")) {
        std::cerr << "Error: Data directory not found. Please ensure the 'data' directory is present." << std::endl;
        exit(1);
    }
}

int combat_encounter(GameWindow &main_window, Character &player, std::string enemy_type) {
    Monster opponent;
    if (enemy_type == "Random") {
        opponent.random_spawn();
    } else {
        opponent.load(".rpg/creatures/" + enemy_type);
    }
    while(player.get_hp() > 0 && opponent.get_hp() > 0) {
        std::vector<std::map<std::string, std::string>> combat_action = main_window.render_combat(player, opponent);
        if (combat_action[0]["type"] == "attack") {
            // Attack
            int damage = player.attack(opponent);
            if(damage > 0) {
            opponent.wound(damage);
                main_window.log("You hit the " + opponent.get_species() + " for " + std::to_string(damage) + " points.");
            }
            else
                main_window.log("With a mighty swing... you completely miss the " + opponent.get_species() + ".");
            if(opponent.get_hp() <= 0) {
                main_window.log("You defeated the " + opponent.get_species() + "!");
                player.award_exp(5);
                main_window.log("Gained 5 EXP!");
                return 1;
            }
            damage = opponent.attack(player);
            if(damage > 0) {
            player.wound(damage);
                main_window.log("The " + opponent.get_species() + " hits you for " + std::to_string(damage) + " points.");
            }
            else
                main_window.log("You deftly dodge the " + opponent.get_species() + "'s attack!.");
            if(player.get_hp() <= 0) {
                main_window.log("You were defeated by the " + opponent.get_species() + "...");
                main_window.log("Game Over.");
                return 0;
            }
        }
        else if (combat_action[0]["type"] == "escape") {
            main_window.log("You successfully escaped from the " + opponent.get_species() + "!");
            return 2;
        }
    }
    return -1; // This should never happen, but just to satisfy the compiler. Indicates an error in combat logic if it does.
}


int main(int argc, char *argv[]) {

    // Make sure the data directory exists and is populated, if not copy from the provided template
    validate_data_dir();
    // Setup TUI
    GameWindow main_window = GameWindow();
    // Character selection / creation
    std::vector<std::string> characters = creature_list(".rpg/creatures/characters");
    Character player = Character();
    characters.push_back("[New Character]");
    int char_index = main_window.list_select("Select Character", characters);
    if (char_index == characters.size() - 1) {
        // Create new character flow (not implemented yet)
        std::cerr << "New character creation not implemented yet. Exiting." << std::endl;
        return 1;
    }
    player.load(".rpg/creatures/characters/" + characters[char_index]);
    main_window.log("Loaded character: " + player.get_name());
    // Load starting location
    Location current_location = Location();
    current_location.load(".rpg/locations/" + player.get_location());

    std::vector<std::map<std::string, std::string>> action_results;

    // Main game loop
    while (action_results.empty() || action_results[0]["type"] != "quit") {
        action_results = main_window.render_scene(player, current_location);
        // Get a random result from the selected action's possible results
        std::vector<std::map<std::string, std::string>> chosen_results;
        if (!action_results.empty()) {
            std::sample(action_results.begin(),
                        action_results.end(),
                        std::back_inserter(chosen_results),
                        1, std::mt19937{std::random_device{}()});
        }
        const auto &result = chosen_results.empty() ? std::map<std::string, std::string>{} : chosen_results[0];
        if (result.at("type") == "move") {
            current_location.load(".rpg/locations/" + result.at("location"));
            player.set_location(result.at("location"));
            main_window.log("Moved to: " + result.at("location"));
        }
        // Combat Encounter loop
        else if (result.at("type") == "combat") {
            // Refactor this into a function
            int combat_result = combat_encounter(main_window, player, result.at("enemy"));
            if (combat_result == 0) {
                // Player was defeated
                current_location.load(".rpg/locations/Start");
                player.set_location("Start");
                break;
            }
        }
        else if (result.at("type") == "heal") {
            int exp_cost = std::stoi(result.at("exp_cost"));
            if (player.consume_exp(exp_cost)) {
                player.heal_direct();
                main_window.log("You spent " + std::to_string(exp_cost) + " EXP to restore your health!");
            } else {
                main_window.log("You don't have enough EXP to pay for healing. You need " + std::to_string(exp_cost) + " EXP.");
            }
        }
        else if (result.at("type") == "none") {
            // Nothing
        }
        else {
            main_window.log("Unknown action result type: " + result.at("type"));
        }
    }
    // Save character on exit
    player.save();
    return 0;
    //Character player = character_select(characters);
    //player.equip("main_hand", std::make_shared<Weapon>("Sword", "Weapon", 10, 3.0, 100, 5, "Slashing"));
    //main_screen(player);
}
