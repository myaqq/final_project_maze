# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/HelloSFML.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HelloSFML.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HelloSFML.dir/flags.make

CMakeFiles/HelloSFML.dir/ver2.cpp.o: CMakeFiles/HelloSFML.dir/flags.make
CMakeFiles/HelloSFML.dir/ver2.cpp.o: ../ver2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HelloSFML.dir/ver2.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HelloSFML.dir/ver2.cpp.o -c /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/ver2.cpp

CMakeFiles/HelloSFML.dir/ver2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HelloSFML.dir/ver2.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/ver2.cpp > CMakeFiles/HelloSFML.dir/ver2.cpp.i

CMakeFiles/HelloSFML.dir/ver2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HelloSFML.dir/ver2.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/ver2.cpp -o CMakeFiles/HelloSFML.dir/ver2.cpp.s

# Object files for target HelloSFML
HelloSFML_OBJECTS = \
"CMakeFiles/HelloSFML.dir/ver2.cpp.o"

# External object files for target HelloSFML
HelloSFML_EXTERNAL_OBJECTS =

HelloSFML: CMakeFiles/HelloSFML.dir/ver2.cpp.o
HelloSFML: CMakeFiles/HelloSFML.dir/build.make
HelloSFML: /usr/local/lib/libsfml-graphics.2.5.1.dylib
HelloSFML: /usr/local/lib/libsfml-audio.2.5.1.dylib
HelloSFML: /usr/local/lib/libsfml-network.2.5.1.dylib
HelloSFML: /usr/local/lib/libsfml-window.2.5.1.dylib
HelloSFML: /usr/local/lib/libsfml-system.2.5.1.dylib
HelloSFML: CMakeFiles/HelloSFML.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HelloSFML"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HelloSFML.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HelloSFML.dir/build: HelloSFML

.PHONY : CMakeFiles/HelloSFML.dir/build

CMakeFiles/HelloSFML.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HelloSFML.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HelloSFML.dir/clean

CMakeFiles/HelloSFML.dir/depend:
	cd /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝 /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝 /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/cmake-build-debug /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/cmake-build-debug /Users/eric/Documents/GitHub/final_project_maze/0109_1506拷貝/cmake-build-debug/CMakeFiles/HelloSFML.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HelloSFML.dir/depend

