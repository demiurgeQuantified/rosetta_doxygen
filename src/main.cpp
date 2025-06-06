#include <iostream>
#include <filesystem>

#include "rosetta_doxygen/converter.h"
#include "rosetta_doxygen/rosetta/parser.h"
#include "rosetta_doxygen/rosetta/lua/types.h"

using namespace rosetta_doxygen;
using namespace rosetta_doxygen::rosetta;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    const std::filesystem::path inputPath = {argv[1]};

    if (!std::filesystem::exists(inputPath)
        || !std::filesystem::is_regular_file(inputPath)) {
        return 1;
    }

    // rosetta_doxygen::rosetta::lua::class_ testClass{
    //     "Test",
    //     "Debug class.",
    //     {"classA", "classB"},
    //     {
    //         {
    //             "staticField",
    //             "string",
    //             "Debug field.",
    //             true
    //         },
    //         {
    //             "field",
    //             "number",
    //             "Test field.",
    //             false
    //         }
    //     },
    //     {
    //         {
    //             "staticFunction",
    //             {
    //                 "nil",
    //                 "NIL SHOULD NOT BE DOCUMENTED"
    //             },
    //             {
    //                 {
    //                     "s",
    //                     "string",
    //                     "A string parameter."
    //                 },
    //                 {
    //                     "n",
    //                     "number",
    //                     "A number parameter."
    //                 }
    //             },
    //             "A static function."
    //         },
    //         {
    //             "instanceFunction",
    //             {
    //                 "string",
    //                 "The resulting string."
    //             },
    //             {
    //                 {
    //                     "self",
    //                     "Test",
    //                     "SELF SHOULD NOT BE DOCUMENTED"
    //                 },
    //                 {
    //                     "parameter",
    //                     "boolean",
    //                     "A boolean parameter."
    //                 }
    //             },
    //             "An instance function."
    //         }
    //     }
    // };
    //
    // std::cout << rosetta_doxygen::converter::to_c_source(testClass);

    lua::environment environment = parser::parse_yaml(inputPath);

    std::cout << converter::to_c_source(environment.classes);
    for (const lua::function &function: environment.functions) {
        std::cout << converter::to_c_source(function) << '\n';
    }

    return 0;
}
