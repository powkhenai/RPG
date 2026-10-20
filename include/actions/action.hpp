#pragma once
#include <string>
#include <toml/value.hpp>
#include <vector>
#include <map>
#include "toml.hpp"

struct Action
{
    std::string name;
    std::string description;
    std::vector<std::map<std::string, std::string>> results;
};

namespace toml {
    template<>
    struct from< Action > {
        static Action from_toml(const value& v) {
            Action a;
            a.name = find<std::string>(v, "name");
            a.description = find<std::string>(v, "description");
            a.results = find<std::vector<std::map<std::string, std::string>>>(v, "results");
            return a;
        }
    };

    template<>
    struct into< Action > {
        static toml::table into_toml(const Action& a) {
            return toml::table{
                {"name", a.name},
                {"description", a.description},
                {"results", a.results}
            };
        }
    };
}
