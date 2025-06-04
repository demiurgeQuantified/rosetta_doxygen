#include "parser.h"

#include <cassert>
#include <fstream>
#include "yaml-cpp/yaml.h"

using namespace rosetta_doxygen::rosetta;

namespace {
    lua::function parse_function(YAML::Node yaml) {
        lua::function function = {};

        if (yaml["name"]) {
            function.name = yaml["name"].as<std::string>();
        }

        if (yaml["return"]) {
            // we ignore multiple returns because they won't parse as c++
            // type unions are also going to be an issue... TODO what can be done about this?
            function.return_.type = yaml["return"][0]["type"].as<std::string>();
        } else {
            function.return_.type = "nil";
        }

        if (yaml["parameters"]) {
            for (auto &&parameter : yaml["parameters"]) {
                function.parameters.emplace_back(
                    parameter["name"].as<std::string>(),
                    parameter["type"] ? parameter["type"].as<std::string>() : "nil",
                    parameter["notes"] ? parameter["notes"].as<std::string>() : ""
                );
            }
        }

        return function;
    }
}

std::vector<lua::class_> parser::parse_yaml(std::filesystem::path filePath) {
    assert(std::filesystem::exists(filePath) and std::filesystem::is_regular_file(filePath));
    std::fstream stream {filePath, std::fstream::in};

    YAML::Node rosettaFile = YAML::Load(stream);
    if (!rosettaFile["version"] or rosettaFile["version"].as<std::string>() != "1.1") {
        return {};
    }

    if (!rosettaFile["languages"]
        or !rosettaFile["languages"]["lua"]
        or !rosettaFile["languages"]["lua"]["classes"])
    {
        return {};
    }

    YAML::Node classesNode = rosettaFile["languages"]["lua"]["classes"];

    std::vector<lua::class_> classes;
    classes.reserve(classesNode.size());

    for (YAML::const_iterator it=classesNode.begin(); it != classesNode.end(); ++it) {
        lua::class_ class_ = {
            it->first.as<std::string>()
        };

        if (it->second["extends"]) {
            class_.supers = {it->second["extends"].as<std::string>()};
        }

        YAML::Node methods = it->second["methods"];
        if (methods) {
            for (auto &&method : methods) {
                lua::function function = parse_function(static_cast<YAML::Node>(method));

                // add self to front of parameter list
                std::vector<lua::function::parameter> parameters;
                parameters.push_back(
                    {
                        "self",
                        class_.name
                    }
                );
                for (const lua::function::parameter &parameter : function.parameters) {
                    parameters.push_back(parameter);
                }
                function.parameters = parameters;

                class_.functions.push_back(function);
            }
        }

        YAML::Node staticMethods = it->second["staticMethods"];
        if (staticMethods) {
            for (auto &&method : staticMethods) {
                class_.functions.push_back(
                    parse_function(static_cast<YAML::Node>(method)));
            }
        }

        YAML::Node constructors = it->second["constructors"];
        if (constructors) {
            for (auto &&constructor : constructors) {
                lua::function function = parse_function(static_cast<YAML::Node>(constructor));

                function.name = "new";
                function.return_ = {
                    class_.name
                };

                class_.functions.push_back(function);
            }
        }

        YAML::Node fields = it->second["fields"];
        if (fields) {
            for (YAML::const_iterator it=fields.begin(); it != fields.end(); ++it) {
                // inherited fields don't have a type
                if (!it->second["type"]) {
                    continue;
                }
                class_.fields.emplace_back(
                    it->first.as<std::string>(),
                    it->second["type"].as<std::string>(),
                    it->second["notes"] ? it->second["notes"].as<std::string>() : "",
                    false
                );
            }
        }

        YAML::Node staticFields = it->second["staticFields"];
        if (staticFields) {
            for (YAML::const_iterator it=staticFields.begin(); it != staticFields.end(); ++it) {
                // inherited fields don't have a type
                if (!it->second["type"]) {
                    continue;
                }
                class_.fields.emplace_back(
                    it->first.as<std::string>(),
                    it->second["type"].as<std::string>(),
                    it->second["notes"] ? it->second["notes"].as<std::string>() : "",
                    true
                );
            }
        }

        classes.push_back(class_);
    }

    return classes;
}

