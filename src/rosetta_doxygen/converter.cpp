#include "converter.h"

#include "docstring_builder.h"

using namespace rosetta_doxygen;
using namespace rosetta::lua;


namespace {
    // we need to sanitise type names to be validish c++ or doxygen will get confused
    std::string sanitise_type_name(std::string name) {
        size_t pos = 0;
        while ((pos = name.find('.', pos)) != std::string::npos) {
            name = name.substr(0, pos) + "::" + name.substr(pos + 1);
            pos += 2;
        }
        return name;
    }
}


std::string converter::to_c_source(const function &function) {
    std::string result;

    docstring_builder docstring;
    if (!function.notes.empty()) {
        docstring.add_line(function.notes);
    }

    const bool static_ = function.parameters.empty() or function.parameters[0].name != "self";
    if (static_) {
        result += "static ";
    }

    std::string return_ = sanitise_type_name(function.return_.type);
    if (return_ == "nil") {
        return_ = "void";
    } else {
        if (!function.return_.name.empty()) {
            std::string line = "@retval " + function.return_.name;
            if (!function.return_.notes.empty()) {
                line += " " + function.return_.notes;
            }
            docstring.add_line(line);
        }
        else if (!function.return_.notes.empty()) {
            docstring.add_line("@return " + function.return_.notes);
        }
    }

    result += return_ + " " + function.name + "(";
    for (int i = 0; i < function.parameters.size(); i++) {
        if (i == 0) {
            if (!static_) {
                continue;
            }
        }
        else {
            if (static_ or i > 1) {
                result += ", ";
            }
        }
        function::parameter parameter = function.parameters[i];
        result += sanitise_type_name(parameter.type) + " " + parameter.name;
        if (!parameter.notes.empty()) {
            docstring.add_line("@param " + parameter.name + " " + parameter.notes);
        }
    }

    if (!docstring.empty()) {
        result = docstring.get() + "\n" + result;
    }

    return result += ");";
}


std::string converter::constructor_to_c_source(const class_ &class_, const function &function) {
    docstring_builder docstring;
    if (!function.notes.empty()) {
        docstring.add_line(function.notes);
    }

    std::string result = sanitise_type_name(class_.name) + "(";
    for (int i = 0; i < function.parameters.size(); i++) {
        if (i > 0) {
            result += ", ";
        }
        function::parameter parameter = function.parameters[i];
        result += sanitise_type_name(parameter.type) + " " + parameter.name;
        if (!parameter.notes.empty()) {
            docstring.add_line("@param " + parameter.name + " " + parameter.notes);
        }
    }

    if (!docstring.empty()) {
        result = docstring.get() + "\n" + result;
    }

    return result += ");";
}


std::string converter::to_c_source(const class_ &class_) {
    std::string result;

    docstring_builder docstring;
    if (!class_.notes.empty()) {
        docstring.add_line(class_.notes);
    }
    if (!docstring.empty()) {
        result += docstring.get() + "\n";
    }

    result += "class " + sanitise_type_name(class_.name);
    if (!class_.supers.empty()) {
        result += " : public " + class_.supers[0];
        for (int i = 1; i < class_.supers.size(); i++) {
            result += ", public " + class_.supers[i];
        }
    }
    result += " {\npublic:\n";

    for (const class_::field &field : class_.fields) {
        if (!field.notes.empty()) {
            result += "/// " + field.notes + "\n";
        }
        if (field.static_) {
            result += "static ";
        }
        result += sanitise_type_name(field.type) + " " + field.name + ";\n";
    }

    for (const function &function : class_.functions) {
        if (function.name == "new" and function.return_.type == class_.name) {
            result += constructor_to_c_source(class_, function) + "\n";
        } else {
            result += to_c_source(function) + "\n";
        }
    }

    return result + "};";
}


std::string converter::to_c_source(const std::vector<class_> &classes) {
    std::string result;
    for (const class_ &class_ : classes) {
        result += to_c_source(class_) + "\n";
    }
    return result;
}
