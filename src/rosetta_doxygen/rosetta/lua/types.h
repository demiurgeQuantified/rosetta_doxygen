#pragma once

#include <string>
#include <vector>


/// Lua types
namespace rosetta_doxygen::rosetta::lua {
    /// A Lua function.
    struct function {
        /// A function's parameter.
        struct parameter {
            /// Name of the parameter.
            std::string name;
            /// String name of the expected type of the parameter.
            std::string type;
            /// Additional documentation of the parameter.
            std::string notes;
        };
        /// A function's return signature.
        struct return_ {
            /// String name of the return type of the function.
            std::string type;
            /// Name of the return.
            std::string name;
            /// Additional documentation of the return value.
            std::string notes;
        };
        /// Name of the function.
        std::string name;
        /// The function's return signature.
        return_ return_;
        /// The function's parameters.
        std::vector<parameter> parameters;
        /// Additional documentation of the function.
        std::string notes;
    };


    /// A Lua class.
    struct class_ {
        /// A field belonging to a class.
        struct field {
            /// Name of the field.
            std::string name;
            /// String name of the type of the field.
            std::string type;
            /// Additional documentation of the field.
            std::string notes;
            /// Whether the field is static.
            bool static_;
        };

        /// Name of the class.
        std::string name;
        /// Additional documentation of the class.
        std::string notes;

        /// List of string names of superclasses.
        std::vector<std::string> supers;

        /// List of all fields belonging to this class.
        std::vector<field> fields;
        /// List of all functions belonging to this class.
        std::vector<function> functions;
    };
}
