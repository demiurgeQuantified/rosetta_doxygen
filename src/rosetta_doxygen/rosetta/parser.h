#pragma once

#include <filesystem>

#include "lua/types.h"


namespace rosetta_doxygen::rosetta::parser {
     /**
     * Parses a YAML file and returns any Lua classes contained within.
     * @param filePath Path to the file to parse.
     * @return The classes in the file.
     */
    [[nodiscard]] std::vector<lua::class_> parse_yaml(std::filesystem::path filePath);
}
