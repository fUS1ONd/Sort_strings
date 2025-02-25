# Strings Sorter
The sort_strings utility is designed to sort strings in a file and output them to standard output

## Features
The utility supports two types of working with a file:
• Via File I/O API
• Through file mapping into memory
Also supports 5 types of sortings: `bubble`, `merge`, `insertion`, `radix` and `quick`
At least supports sorting in ascending(`asc`) and descending order(`des`)

The parameter for the number of rows to sort in [1:MaxNumOfLinesInFile]
To find out how many lines are in a file you can use:
1. in linux: `wc -l inputfile.in`
2. in windows: `(Get-Content C:\inputfile.in).Length`

## Usage
common use:
   `./sort_strings number_of_str's input.in option_sorting comparator`

This project has the ability to automatically build and test, implemented using CMake and CTest
Manually utility based on i/o api
To build project(and configure the type of working with file), test and clean must be in the project root then :
1. To create build dir use:
2. if you want to configure util with Map type use:
   `cmake -B build -DUSE_MMAP=ON` and `cmake --build build`
   or do not specify the parameter
   then you can find in `build/` executable file to use according to the usage or
4. Run tests with:
   `cmake --build build --target run_all_tests`

After finishing work, to delete the build directory use:
   `cmake --build build --target clean-build`
   or `rm -rf ./build`
