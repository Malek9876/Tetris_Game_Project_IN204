# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/build

# Include any dependencies generated for this target.
include CMakeFiles/3D_Tetris.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/3D_Tetris.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/3D_Tetris.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/3D_Tetris.dir/flags.make

CMakeFiles/3D_Tetris.dir/src/main.cpp.o: CMakeFiles/3D_Tetris.dir/flags.make
CMakeFiles/3D_Tetris.dir/src/main.cpp.o: /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/src/main.cpp
CMakeFiles/3D_Tetris.dir/src/main.cpp.o: CMakeFiles/3D_Tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/3D_Tetris.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/3D_Tetris.dir/src/main.cpp.o -MF CMakeFiles/3D_Tetris.dir/src/main.cpp.o.d -o CMakeFiles/3D_Tetris.dir/src/main.cpp.o -c /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/src/main.cpp

CMakeFiles/3D_Tetris.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/3D_Tetris.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/src/main.cpp > CMakeFiles/3D_Tetris.dir/src/main.cpp.i

CMakeFiles/3D_Tetris.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/3D_Tetris.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/src/main.cpp -o CMakeFiles/3D_Tetris.dir/src/main.cpp.s

CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o: CMakeFiles/3D_Tetris.dir/flags.make
CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o: /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/src/utils/utils.cpp
CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o: CMakeFiles/3D_Tetris.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o -MF CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o.d -o CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o -c /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/src/utils/utils.cpp

CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/src/utils/utils.cpp > CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.i

CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/src/utils/utils.cpp -o CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.s

# Object files for target 3D_Tetris
3D_Tetris_OBJECTS = \
"CMakeFiles/3D_Tetris.dir/src/main.cpp.o" \
"CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o"

# External object files for target 3D_Tetris
3D_Tetris_EXTERNAL_OBJECTS =

3D_Tetris: CMakeFiles/3D_Tetris.dir/src/main.cpp.o
3D_Tetris: CMakeFiles/3D_Tetris.dir/src/utils/utils.cpp.o
3D_Tetris: CMakeFiles/3D_Tetris.dir/build.make
3D_Tetris: /usr/lib/x86_64-linux-gnu/libGL.so
3D_Tetris: /usr/lib/x86_64-linux-gnu/libglut.so
3D_Tetris: /usr/lib/x86_64-linux-gnu/libGLEW.so
3D_Tetris: /usr/lib/x86_64-linux-gnu/libglfw.so.3.3
3D_Tetris: /usr/lib/x86_64-linux-gnu/libXmu.so
3D_Tetris: /usr/lib/x86_64-linux-gnu/libXi.so
3D_Tetris: CMakeFiles/3D_Tetris.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable 3D_Tetris"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/3D_Tetris.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/3D_Tetris.dir/build: 3D_Tetris
.PHONY : CMakeFiles/3D_Tetris.dir/build

CMakeFiles/3D_Tetris.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/3D_Tetris.dir/cmake_clean.cmake
.PHONY : CMakeFiles/3D_Tetris.dir/clean

CMakeFiles/3D_Tetris.dir/depend:
	cd /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/build /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/build /home/elyes/Documents/GitHub/Tetris_Game_Project_IN204/3d-tetris/build/CMakeFiles/3D_Tetris.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/3D_Tetris.dir/depend

