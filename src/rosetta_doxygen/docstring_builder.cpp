#include "docstring_builder.h"

using rosetta_doxygen::docstring_builder;


void docstring_builder::add_line(const std::string &line) {
    this->lines.emplace_back(line);
}


std::string docstring_builder::get() const {
    if (this->lines.empty()) {
        return "";
    }

    std::string result = "/**\n";

    for (const std::string &line : this->lines) {
        result += " * " + line + "\n";
    }

    return result + " */";
}


bool docstring_builder::empty() const {
    return this->lines.empty();
}
