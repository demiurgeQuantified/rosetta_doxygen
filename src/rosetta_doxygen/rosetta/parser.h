#pragma once

#include <filesystem>

#include "lua/types.h"


namespace rosetta_doxygen::rosetta::parser {
     /**
     * Parses a YAML file and returns the Lua objects contained within.
     * @param filePath Path to the file to parse.
     * @return The classes in the file.
     */
    [[nodiscard]] lua::environment parse_yaml(std::filesystem::path filePath);
}
