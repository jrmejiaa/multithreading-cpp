# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/build

# Include any dependencies generated for this target.
include CMakeFiles/03_exercise_basic.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/03_exercise_basic.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/03_exercise_basic.dir/flags.make

CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.o: CMakeFiles/03_exercise_basic.dir/flags.make
CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.o: ../03_exercise/shared_value_basic.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.o -c /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/03_exercise/shared_value_basic.cpp

CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/03_exercise/shared_value_basic.cpp > CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.i

CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/03_exercise/shared_value_basic.cpp -o CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.s

# Object files for target 03_exercise_basic
03_exercise_basic_OBJECTS = \
"CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.o"

# External object files for target 03_exercise_basic
03_exercise_basic_EXTERNAL_OBJECTS =

03_exercise_basic: CMakeFiles/03_exercise_basic.dir/03_exercise/shared_value_basic.cpp.o
03_exercise_basic: CMakeFiles/03_exercise_basic.dir/build.make
03_exercise_basic: CMakeFiles/03_exercise_basic.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 03_exercise_basic"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/03_exercise_basic.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/03_exercise_basic.dir/build: 03_exercise_basic

.PHONY : CMakeFiles/03_exercise_basic.dir/build

CMakeFiles/03_exercise_basic.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/03_exercise_basic.dir/cmake_clean.cmake
.PHONY : CMakeFiles/03_exercise_basic.dir/clean

CMakeFiles/03_exercise_basic.dir/depend:
	cd /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/build /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/build /home/jairom/Documents/TUD_Projects/03_Semester/multithreading-cpp/exercises/build/CMakeFiles/03_exercise_basic.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/03_exercise_basic.dir/depend

