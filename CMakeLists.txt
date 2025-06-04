cmake_minimum_required(VERSION 3.31)
project(rosetta_doxygen)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")


add_executable(rosetta_doxygen src/main.cpp
        src/rosetta_doxygen/rosetta/lua/types.h
        src/rosetta_doxygen/rosetta/parser.h
        src/rosetta_doxygen/rosetta/parser.cpp
        src/rosetta_doxygen/converter.h
        src/rosetta_doxygen/converter.cpp
        src/rosetta_doxygen/docstring_builder.cpp
        src/rosetta_doxygen/docstring_builder.h)

target_link_libraries(rosetta_doxygen PUBLIC yaml-cpp::yaml-cpp) # The library or executable that require yaml-cpp library
