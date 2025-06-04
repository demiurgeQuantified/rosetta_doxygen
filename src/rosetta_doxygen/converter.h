#pragma once

#include <string>
#include <vector>

#include "rosetta/lua/types.h"


namespace rosetta_doxygen::converter {
    /**
     * Represents a Lua function as C++ source code.
     * @param function The function to represent.
     * @return C++ source code representing the function.
     */
    [[nodiscard]] std::string to_c_source(const rosetta::lua::function &function);

    /**
     * Represents a Lua function as a constructor in C++ source code.
     * @param function The function to represent.
     * @return C++ source code of a constructor representing the function.
     */
    [[nodiscard]] std::string constructor_to_c_source(
        const rosetta::lua::class_ &class_,
        const rosetta::lua::function &function
    );

    /**
     * Represents a Lua class as C++ source code.
     * @param class_ The class to represent.
     * @return C++ source code representing the class.
     */
    [[nodiscard]] std::string to_c_source(const rosetta::lua::class_ &class_);

    /**
     * Represents a vector of Lua classes as C++ source code.
     * @param classes The classes to represent.
     * @return C++ source code representing the classes.
     */
    [[nodiscard]] std::string to_c_source(const std::vector<rosetta::lua::class_> &classes);
}
