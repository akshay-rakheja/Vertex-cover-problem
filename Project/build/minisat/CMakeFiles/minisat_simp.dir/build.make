# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages/cmake/data/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages/cmake/data/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build

# Include any dependencies generated for this target.
include minisat/CMakeFiles/minisat_simp.dir/depend.make

# Include the progress variables for this target.
include minisat/CMakeFiles/minisat_simp.dir/progress.make

# Include the compile flags for this target's objects.
include minisat/CMakeFiles/minisat_simp.dir/flags.make

minisat/CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.o: minisat/CMakeFiles/minisat_simp.dir/flags.make
minisat/CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.o: ../minisat/minisat/simp/Main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object minisat/CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.o"
	cd /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/minisat && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.o -c /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/minisat/minisat/simp/Main.cc

minisat/CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.i"
	cd /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/minisat && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/minisat/minisat/simp/Main.cc > CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.i

minisat/CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.s"
	cd /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/minisat && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/minisat/minisat/simp/Main.cc -o CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.s

# Object files for target minisat_simp
minisat_simp_OBJECTS = \
"CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.o"

# External object files for target minisat_simp
minisat_simp_EXTERNAL_OBJECTS =

minisat/minisat: minisat/CMakeFiles/minisat_simp.dir/minisat/simp/Main.cc.o
minisat/minisat: minisat/CMakeFiles/minisat_simp.dir/build.make
minisat/minisat: minisat/libminisat.a
minisat/minisat: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX11.1.sdk/usr/lib/libz.tbd
minisat/minisat: minisat/CMakeFiles/minisat_simp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable minisat"
	cd /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/minisat && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/minisat_simp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
minisat/CMakeFiles/minisat_simp.dir/build: minisat/minisat

.PHONY : minisat/CMakeFiles/minisat_simp.dir/build

minisat/CMakeFiles/minisat_simp.dir/clean:
	cd /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/minisat && $(CMAKE_COMMAND) -P CMakeFiles/minisat_simp.dir/cmake_clean.cmake
.PHONY : minisat/CMakeFiles/minisat_simp.dir/clean

minisat/CMakeFiles/minisat_simp.dir/depend:
	cd /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/minisat /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/minisat /Users/akshayrakheja/Documents/ECE650/Project/satoshi/project/build/minisat/CMakeFiles/minisat_simp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : minisat/CMakeFiles/minisat_simp.dir/depend
