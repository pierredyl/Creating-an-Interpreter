# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dylanpierre/Desktop/Creating-an-Interpreter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Creating_an_Interpreter.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Creating_an_Interpreter.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Creating_an_Interpreter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Creating_an_Interpreter.dir/flags.make

CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/flags.make
CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o: /Users/dylanpierre/Desktop/Creating-an-Interpreter/main.cpp
CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o -MF CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o.d -o CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o -c /Users/dylanpierre/Desktop/Creating-an-Interpreter/main.cpp

CMakeFiles/Creating_an_Interpreter.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Creating_an_Interpreter.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dylanpierre/Desktop/Creating-an-Interpreter/main.cpp > CMakeFiles/Creating_an_Interpreter.dir/main.cpp.i

CMakeFiles/Creating_an_Interpreter.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Creating_an_Interpreter.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dylanpierre/Desktop/Creating-an-Interpreter/main.cpp -o CMakeFiles/Creating_an_Interpreter.dir/main.cpp.s

CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/flags.make
CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o: /Users/dylanpierre/Desktop/Creating-an-Interpreter/remove_comments.cpp
CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o -MF CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o.d -o CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o -c /Users/dylanpierre/Desktop/Creating-an-Interpreter/remove_comments.cpp

CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dylanpierre/Desktop/Creating-an-Interpreter/remove_comments.cpp > CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.i

CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dylanpierre/Desktop/Creating-an-Interpreter/remove_comments.cpp -o CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.s

CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/flags.make
CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o: /Users/dylanpierre/Desktop/Creating-an-Interpreter/Tokenizer.cpp
CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o -MF CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o.d -o CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o -c /Users/dylanpierre/Desktop/Creating-an-Interpreter/Tokenizer.cpp

CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dylanpierre/Desktop/Creating-an-Interpreter/Tokenizer.cpp > CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.i

CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dylanpierre/Desktop/Creating-an-Interpreter/Tokenizer.cpp -o CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.s

CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/flags.make
CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o: /Users/dylanpierre/Desktop/Creating-an-Interpreter/Token.cpp
CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o -MF CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o.d -o CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o -c /Users/dylanpierre/Desktop/Creating-an-Interpreter/Token.cpp

CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dylanpierre/Desktop/Creating-an-Interpreter/Token.cpp > CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.i

CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dylanpierre/Desktop/Creating-an-Interpreter/Token.cpp -o CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.s

CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/flags.make
CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o: /Users/dylanpierre/Desktop/Creating-an-Interpreter/RecursiveDescentParser.cpp
CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o -MF CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o.d -o CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o -c /Users/dylanpierre/Desktop/Creating-an-Interpreter/RecursiveDescentParser.cpp

CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dylanpierre/Desktop/Creating-an-Interpreter/RecursiveDescentParser.cpp > CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.i

CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dylanpierre/Desktop/Creating-an-Interpreter/RecursiveDescentParser.cpp -o CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.s

CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/flags.make
CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o: /Users/dylanpierre/Desktop/Creating-an-Interpreter/SymbolTable.cpp
CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o: CMakeFiles/Creating_an_Interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o -MF CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o.d -o CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o -c /Users/dylanpierre/Desktop/Creating-an-Interpreter/SymbolTable.cpp

CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dylanpierre/Desktop/Creating-an-Interpreter/SymbolTable.cpp > CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.i

CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dylanpierre/Desktop/Creating-an-Interpreter/SymbolTable.cpp -o CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.s

# Object files for target Creating_an_Interpreter
Creating_an_Interpreter_OBJECTS = \
"CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o" \
"CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o" \
"CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o" \
"CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o" \
"CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o" \
"CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o"

# External object files for target Creating_an_Interpreter
Creating_an_Interpreter_EXTERNAL_OBJECTS =

Creating_an_Interpreter: CMakeFiles/Creating_an_Interpreter.dir/main.cpp.o
Creating_an_Interpreter: CMakeFiles/Creating_an_Interpreter.dir/remove_comments.cpp.o
Creating_an_Interpreter: CMakeFiles/Creating_an_Interpreter.dir/Tokenizer.cpp.o
Creating_an_Interpreter: CMakeFiles/Creating_an_Interpreter.dir/Token.cpp.o
Creating_an_Interpreter: CMakeFiles/Creating_an_Interpreter.dir/RecursiveDescentParser.cpp.o
Creating_an_Interpreter: CMakeFiles/Creating_an_Interpreter.dir/SymbolTable.cpp.o
Creating_an_Interpreter: CMakeFiles/Creating_an_Interpreter.dir/build.make
Creating_an_Interpreter: CMakeFiles/Creating_an_Interpreter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable Creating_an_Interpreter"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Creating_an_Interpreter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Creating_an_Interpreter.dir/build: Creating_an_Interpreter
.PHONY : CMakeFiles/Creating_an_Interpreter.dir/build

CMakeFiles/Creating_an_Interpreter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Creating_an_Interpreter.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Creating_an_Interpreter.dir/clean

CMakeFiles/Creating_an_Interpreter.dir/depend:
	cd /Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dylanpierre/Desktop/Creating-an-Interpreter /Users/dylanpierre/Desktop/Creating-an-Interpreter /Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug /Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug /Users/dylanpierre/Desktop/Creating-an-Interpreter/cmake-build-debug/CMakeFiles/Creating_an_Interpreter.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Creating_an_Interpreter.dir/depend

