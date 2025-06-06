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
            YAML::Node yamlReturn = yaml["return"][0];
            function.return_.type = yamlReturn["type"].as<std::string>();
            function.return_.name = yamlReturn["name"] ? yamlReturn["name"].as<std::string>() : "";
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

lua::environment parser::parse_yaml(std::filesystem::path filePath) {
    assert(std::filesystem::exists(filePath) and std::filesystem::is_regular_file(filePath));
    std::fstream stream {filePath, std::fstream::in};

    lua::environment environment;

    YAML::Node rosettaFile = YAML::Load(stream);
    if (!rosettaFile["version"] or rosettaFile["version"].as<std::string>() != "1.1") {
        return environment;
    }

    if (!rosettaFile["languages"] or !rosettaFile["languages"]["lua"])
    {
        return environment;
    }

    YAML::Node luaNode = rosettaFile["languages"]["lua"];

    if (YAML::Node classesNode = luaNode["classes"]) {
        environment.classes.reserve(classesNode.size());

        for (YAML::const_iterator it=classesNode.begin(); it != classesNode.end(); ++it) {
            lua::class_ class_ = {
                it->first.as<std::string>()
            };

            if (it->second["extends"]) {
                class_.supers = {it->second["extends"].as<std::string>()};
            }

            if (YAML::Node methods = it->second["methods"]) {
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

            if (YAML::Node staticMethods = it->second["staticMethods"]) {
                for (auto &&method : staticMethods) {
                    class_.functions.push_back(
                        parse_function(static_cast<YAML::Node>(method)));
                }
            }

            if (YAML::Node constructors = it->second["constructors"]) {
                for (auto &&constructor : constructors) {
                    lua::function function = parse_function(static_cast<YAML::Node>(constructor));

                    function.name = "new";
                    function.return_ = {
                        class_.name
                    };

                    class_.functions.push_back(function);
                }
            }

            if (YAML::Node fields = it->second["fields"]) {
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

            if (YAML::Node staticFields = it->second["staticFields"]) {
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

            environment.classes.push_back(class_);
        }
    }

    if (YAML::Node functionsNode = luaNode["functions"]) {
        environment.functions.reserve(functionsNode.size());

        for (auto &&function : functionsNode) {
            environment.functions.push_back(
                parse_function(static_cast<YAML::Node>(function)));
        }
    }

    return environment;
}

