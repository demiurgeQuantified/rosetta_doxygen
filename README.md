# Rosetta Doxygen
Doxygen filter for Rosetta data.

## Usage
1) Build with CMake.
2) Place binary next to Doxygen.
3) Configure Doxygen:
   - Add `.yml` to `FILE_PATTERNS`
   - Add `*.yml=rosetta_doxygen` to `FILTER_PATTERNS`
   - Add `yml=c++` to `EXTENSION_MAPPING`
4) Run Doxygen on your Rosetta data.
   - Only YAML format is currently supported.
   - Documentation is currently only created for Lua classes.
