#include <iostream>

#include "location/location.hpp"

int main(int argc, char *argv[]) {
    Location loc;
    loc.load(".rpg/locations/Start");
    std::cout << "Location: " << loc.get_name() << std::endl;
    std::cout << "Description: " << loc.get_description() << std::endl;
    std::cout << "Actions:" << std::endl;
    for (const Action &action : loc.get_actions()) {
        std::cout << "- " << action.name << ": " << action.description << std::endl;
        for (const auto &result : action.results) {
            std::cout << "  Result type: " << result.at("type") << std::endl;
            if (result.at("type") == "move") {
                std::cout << "  Move to location: " << result.at("location") << std::endl;
            }
        }
    }
    return 0;
}
