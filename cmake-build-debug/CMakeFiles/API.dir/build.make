# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /mnt/c/Users/39392/CLionProjects/API

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/39392/CLionProjects/API/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/API.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/API.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/API.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/API.dir/flags.make

CMakeFiles/API.dir/main.c.o: CMakeFiles/API.dir/flags.make
CMakeFiles/API.dir/main.c.o: ../main.c
CMakeFiles/API.dir/main.c.o: CMakeFiles/API.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/39392/CLionProjects/API/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/API.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/API.dir/main.c.o -MF CMakeFiles/API.dir/main.c.o.d -o CMakeFiles/API.dir/main.c.o -c /mnt/c/Users/39392/CLionProjects/API/main.c

CMakeFiles/API.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/API.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/39392/CLionProjects/API/main.c > CMakeFiles/API.dir/main.c.i

CMakeFiles/API.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/API.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/39392/CLionProjects/API/main.c -o CMakeFiles/API.dir/main.c.s

CMakeFiles/API.dir/test.c.o: CMakeFiles/API.dir/flags.make
CMakeFiles/API.dir/test.c.o: ../test.c
CMakeFiles/API.dir/test.c.o: CMakeFiles/API.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/39392/CLionProjects/API/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/API.dir/test.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/API.dir/test.c.o -MF CMakeFiles/API.dir/test.c.o.d -o CMakeFiles/API.dir/test.c.o -c /mnt/c/Users/39392/CLionProjects/API/test.c

CMakeFiles/API.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/API.dir/test.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/39392/CLionProjects/API/test.c > CMakeFiles/API.dir/test.c.i

CMakeFiles/API.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/API.dir/test.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/39392/CLionProjects/API/test.c -o CMakeFiles/API.dir/test.c.s

CMakeFiles/API.dir/strutture_dati.c.o: CMakeFiles/API.dir/flags.make
CMakeFiles/API.dir/strutture_dati.c.o: ../strutture_dati.c
CMakeFiles/API.dir/strutture_dati.c.o: CMakeFiles/API.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/39392/CLionProjects/API/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/API.dir/strutture_dati.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/API.dir/strutture_dati.c.o -MF CMakeFiles/API.dir/strutture_dati.c.o.d -o CMakeFiles/API.dir/strutture_dati.c.o -c /mnt/c/Users/39392/CLionProjects/API/strutture_dati.c

CMakeFiles/API.dir/strutture_dati.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/API.dir/strutture_dati.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/39392/CLionProjects/API/strutture_dati.c > CMakeFiles/API.dir/strutture_dati.c.i

CMakeFiles/API.dir/strutture_dati.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/API.dir/strutture_dati.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/39392/CLionProjects/API/strutture_dati.c -o CMakeFiles/API.dir/strutture_dati.c.s

CMakeFiles/API.dir/ordini.c.o: CMakeFiles/API.dir/flags.make
CMakeFiles/API.dir/ordini.c.o: ../ordini.c
CMakeFiles/API.dir/ordini.c.o: CMakeFiles/API.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/39392/CLionProjects/API/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/API.dir/ordini.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/API.dir/ordini.c.o -MF CMakeFiles/API.dir/ordini.c.o.d -o CMakeFiles/API.dir/ordini.c.o -c /mnt/c/Users/39392/CLionProjects/API/ordini.c

CMakeFiles/API.dir/ordini.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/API.dir/ordini.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/39392/CLionProjects/API/ordini.c > CMakeFiles/API.dir/ordini.c.i

CMakeFiles/API.dir/ordini.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/API.dir/ordini.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/39392/CLionProjects/API/ordini.c -o CMakeFiles/API.dir/ordini.c.s

# Object files for target API
API_OBJECTS = \
"CMakeFiles/API.dir/main.c.o" \
"CMakeFiles/API.dir/test.c.o" \
"CMakeFiles/API.dir/strutture_dati.c.o" \
"CMakeFiles/API.dir/ordini.c.o"

# External object files for target API
API_EXTERNAL_OBJECTS =

API: CMakeFiles/API.dir/main.c.o
API: CMakeFiles/API.dir/test.c.o
API: CMakeFiles/API.dir/strutture_dati.c.o
API: CMakeFiles/API.dir/ordini.c.o
API: CMakeFiles/API.dir/build.make
API: CMakeFiles/API.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/39392/CLionProjects/API/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable API"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/API.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/API.dir/build: API
.PHONY : CMakeFiles/API.dir/build

CMakeFiles/API.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/API.dir/cmake_clean.cmake
.PHONY : CMakeFiles/API.dir/clean

CMakeFiles/API.dir/depend:
	cd /mnt/c/Users/39392/CLionProjects/API/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/39392/CLionProjects/API /mnt/c/Users/39392/CLionProjects/API /mnt/c/Users/39392/CLionProjects/API/cmake-build-debug /mnt/c/Users/39392/CLionProjects/API/cmake-build-debug /mnt/c/Users/39392/CLionProjects/API/cmake-build-debug/CMakeFiles/API.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/API.dir/depend

