#pragma once

#include <string>
#include <vector>

namespace rosetta_doxygen {
    /// Builds a docstring with given lines, handling indentation and syntax automatically.
    class docstring_builder {
    public:
        /// Adds a line to the docstring.
        void add_line(const std::string& line);

        /// Returns the full formatted docstring. If no lines were added, returns an empty string.
        [[nodiscard]] std::string get() const;

        /// Returns true if the docstring is empty.
        [[nodiscard]] bool empty() const;
    private:
        std::string string;
        std::vector<std::string> lines;
    };
}
