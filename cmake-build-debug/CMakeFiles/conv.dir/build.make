# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mari/work/MPPCinstall/cosmiTrigger/conv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/conv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/conv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/conv.dir/flags.make

CMakeFiles/conv.dir/conv2.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/conv2.cpp.o: ../conv2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/conv.dir/conv2.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/conv2.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/conv2.cpp

CMakeFiles/conv.dir/conv2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/conv2.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/conv2.cpp > CMakeFiles/conv.dir/conv2.cpp.i

CMakeFiles/conv.dir/conv2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/conv2.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/conv2.cpp -o CMakeFiles/conv.dir/conv2.cpp.s

CMakeFiles/conv.dir/conv2.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/conv2.cpp.o.requires

CMakeFiles/conv.dir/conv2.cpp.o.provides: CMakeFiles/conv.dir/conv2.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/conv2.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/conv2.cpp.o.provides

CMakeFiles/conv.dir/conv2.cpp.o.provides.build: CMakeFiles/conv.dir/conv2.cpp.o


CMakeFiles/conv.dir/src/TreeManager.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/src/TreeManager.cpp.o: ../src/TreeManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/conv.dir/src/TreeManager.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/src/TreeManager.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/TreeManager.cpp

CMakeFiles/conv.dir/src/TreeManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/src/TreeManager.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/TreeManager.cpp > CMakeFiles/conv.dir/src/TreeManager.cpp.i

CMakeFiles/conv.dir/src/TreeManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/src/TreeManager.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/TreeManager.cpp -o CMakeFiles/conv.dir/src/TreeManager.cpp.s

CMakeFiles/conv.dir/src/TreeManager.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/src/TreeManager.cpp.o.requires

CMakeFiles/conv.dir/src/TreeManager.cpp.o.provides: CMakeFiles/conv.dir/src/TreeManager.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/src/TreeManager.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/src/TreeManager.cpp.o.provides

CMakeFiles/conv.dir/src/TreeManager.cpp.o.provides.build: CMakeFiles/conv.dir/src/TreeManager.cpp.o


CMakeFiles/conv.dir/src/Visualization.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/src/Visualization.cpp.o: ../src/Visualization.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/conv.dir/src/Visualization.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/src/Visualization.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/Visualization.cpp

CMakeFiles/conv.dir/src/Visualization.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/src/Visualization.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/Visualization.cpp > CMakeFiles/conv.dir/src/Visualization.cpp.i

CMakeFiles/conv.dir/src/Visualization.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/src/Visualization.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/Visualization.cpp -o CMakeFiles/conv.dir/src/Visualization.cpp.s

CMakeFiles/conv.dir/src/Visualization.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/src/Visualization.cpp.o.requires

CMakeFiles/conv.dir/src/Visualization.cpp.o.provides: CMakeFiles/conv.dir/src/Visualization.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/src/Visualization.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/src/Visualization.cpp.o.provides

CMakeFiles/conv.dir/src/Visualization.cpp.o.provides.build: CMakeFiles/conv.dir/src/Visualization.cpp.o


CMakeFiles/conv.dir/src/BothReadDetector.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/src/BothReadDetector.cpp.o: ../src/BothReadDetector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/conv.dir/src/BothReadDetector.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/src/BothReadDetector.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/BothReadDetector.cpp

CMakeFiles/conv.dir/src/BothReadDetector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/src/BothReadDetector.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/BothReadDetector.cpp > CMakeFiles/conv.dir/src/BothReadDetector.cpp.i

CMakeFiles/conv.dir/src/BothReadDetector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/src/BothReadDetector.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/BothReadDetector.cpp -o CMakeFiles/conv.dir/src/BothReadDetector.cpp.s

CMakeFiles/conv.dir/src/BothReadDetector.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/src/BothReadDetector.cpp.o.requires

CMakeFiles/conv.dir/src/BothReadDetector.cpp.o.provides: CMakeFiles/conv.dir/src/BothReadDetector.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/src/BothReadDetector.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/src/BothReadDetector.cpp.o.provides

CMakeFiles/conv.dir/src/BothReadDetector.cpp.o.provides.build: CMakeFiles/conv.dir/src/BothReadDetector.cpp.o


CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o: ../src/CosmicMeasSystem.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CosmicMeasSystem.cpp

CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CosmicMeasSystem.cpp > CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.i

CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CosmicMeasSystem.cpp -o CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.s

CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o.requires

CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o.provides: CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o.provides

CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o.provides.build: CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o


CMakeFiles/conv.dir/src/TriggerManager.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/src/TriggerManager.cpp.o: ../src/TriggerManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/conv.dir/src/TriggerManager.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/src/TriggerManager.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/TriggerManager.cpp

CMakeFiles/conv.dir/src/TriggerManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/src/TriggerManager.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/TriggerManager.cpp > CMakeFiles/conv.dir/src/TriggerManager.cpp.i

CMakeFiles/conv.dir/src/TriggerManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/src/TriggerManager.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/TriggerManager.cpp -o CMakeFiles/conv.dir/src/TriggerManager.cpp.s

CMakeFiles/conv.dir/src/TriggerManager.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/src/TriggerManager.cpp.o.requires

CMakeFiles/conv.dir/src/TriggerManager.cpp.o.provides: CMakeFiles/conv.dir/src/TriggerManager.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/src/TriggerManager.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/src/TriggerManager.cpp.o.provides

CMakeFiles/conv.dir/src/TriggerManager.cpp.o.provides.build: CMakeFiles/conv.dir/src/TriggerManager.cpp.o


CMakeFiles/conv.dir/src/CsIManager.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/src/CsIManager.cpp.o: ../src/CsIManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/conv.dir/src/CsIManager.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/src/CsIManager.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CsIManager.cpp

CMakeFiles/conv.dir/src/CsIManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/src/CsIManager.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CsIManager.cpp > CMakeFiles/conv.dir/src/CsIManager.cpp.i

CMakeFiles/conv.dir/src/CsIManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/src/CsIManager.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CsIManager.cpp -o CMakeFiles/conv.dir/src/CsIManager.cpp.s

CMakeFiles/conv.dir/src/CsIManager.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/src/CsIManager.cpp.o.requires

CMakeFiles/conv.dir/src/CsIManager.cpp.o.provides: CMakeFiles/conv.dir/src/CsIManager.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/src/CsIManager.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/src/CsIManager.cpp.o.provides

CMakeFiles/conv.dir/src/CsIManager.cpp.o.provides.build: CMakeFiles/conv.dir/src/CsIManager.cpp.o


CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o: ../src/CosmicRayCounter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CosmicRayCounter.cpp

CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CosmicRayCounter.cpp > CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.i

CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CosmicRayCounter.cpp -o CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.s

CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o.requires

CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o.provides: CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o.provides

CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o.provides.build: CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o


CMakeFiles/conv.dir/src/CsI.cpp.o: CMakeFiles/conv.dir/flags.make
CMakeFiles/conv.dir/src/CsI.cpp.o: ../src/CsI.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/conv.dir/src/CsI.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/conv.dir/src/CsI.cpp.o -c /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CsI.cpp

CMakeFiles/conv.dir/src/CsI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/conv.dir/src/CsI.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CsI.cpp > CMakeFiles/conv.dir/src/CsI.cpp.i

CMakeFiles/conv.dir/src/CsI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/conv.dir/src/CsI.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mari/work/MPPCinstall/cosmiTrigger/conv/src/CsI.cpp -o CMakeFiles/conv.dir/src/CsI.cpp.s

CMakeFiles/conv.dir/src/CsI.cpp.o.requires:

.PHONY : CMakeFiles/conv.dir/src/CsI.cpp.o.requires

CMakeFiles/conv.dir/src/CsI.cpp.o.provides: CMakeFiles/conv.dir/src/CsI.cpp.o.requires
	$(MAKE) -f CMakeFiles/conv.dir/build.make CMakeFiles/conv.dir/src/CsI.cpp.o.provides.build
.PHONY : CMakeFiles/conv.dir/src/CsI.cpp.o.provides

CMakeFiles/conv.dir/src/CsI.cpp.o.provides.build: CMakeFiles/conv.dir/src/CsI.cpp.o


# Object files for target conv
conv_OBJECTS = \
"CMakeFiles/conv.dir/conv2.cpp.o" \
"CMakeFiles/conv.dir/src/TreeManager.cpp.o" \
"CMakeFiles/conv.dir/src/Visualization.cpp.o" \
"CMakeFiles/conv.dir/src/BothReadDetector.cpp.o" \
"CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o" \
"CMakeFiles/conv.dir/src/TriggerManager.cpp.o" \
"CMakeFiles/conv.dir/src/CsIManager.cpp.o" \
"CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o" \
"CMakeFiles/conv.dir/src/CsI.cpp.o"

# External object files for target conv
conv_EXTERNAL_OBJECTS =

conv: CMakeFiles/conv.dir/conv2.cpp.o
conv: CMakeFiles/conv.dir/src/TreeManager.cpp.o
conv: CMakeFiles/conv.dir/src/Visualization.cpp.o
conv: CMakeFiles/conv.dir/src/BothReadDetector.cpp.o
conv: CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o
conv: CMakeFiles/conv.dir/src/TriggerManager.cpp.o
conv: CMakeFiles/conv.dir/src/CsIManager.cpp.o
conv: CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o
conv: CMakeFiles/conv.dir/src/CsI.cpp.o
conv: CMakeFiles/conv.dir/build.make
conv: /Users/mari/local/root/pro/lib/libCore.so
conv: /Users/mari/local/root/pro/lib/libImt.so
conv: /Users/mari/local/root/pro/lib/libRIO.so
conv: /Users/mari/local/root/pro/lib/libNet.so
conv: /Users/mari/local/root/pro/lib/libHist.so
conv: /Users/mari/local/root/pro/lib/libGraf.so
conv: /Users/mari/local/root/pro/lib/libGraf3d.so
conv: /Users/mari/local/root/pro/lib/libGpad.so
conv: /Users/mari/local/root/pro/lib/libTree.so
conv: /Users/mari/local/root/pro/lib/libTreePlayer.so
conv: /Users/mari/local/root/pro/lib/libRint.so
conv: /Users/mari/local/root/pro/lib/libPostscript.so
conv: /Users/mari/local/root/pro/lib/libMatrix.so
conv: /Users/mari/local/root/pro/lib/libPhysics.so
conv: /Users/mari/local/root/pro/lib/libMathCore.so
conv: /Users/mari/local/root/pro/lib/libThread.so
conv: /Users/mari/local/root/pro/lib/libMultiProc.so
conv: CMakeFiles/conv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable conv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/conv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/conv.dir/build: conv

.PHONY : CMakeFiles/conv.dir/build

CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/conv2.cpp.o.requires
CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/src/TreeManager.cpp.o.requires
CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/src/Visualization.cpp.o.requires
CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/src/BothReadDetector.cpp.o.requires
CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/src/CosmicMeasSystem.cpp.o.requires
CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/src/TriggerManager.cpp.o.requires
CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/src/CsIManager.cpp.o.requires
CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/src/CosmicRayCounter.cpp.o.requires
CMakeFiles/conv.dir/requires: CMakeFiles/conv.dir/src/CsI.cpp.o.requires

.PHONY : CMakeFiles/conv.dir/requires

CMakeFiles/conv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/conv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/conv.dir/clean

CMakeFiles/conv.dir/depend:
	cd /Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mari/work/MPPCinstall/cosmiTrigger/conv /Users/mari/work/MPPCinstall/cosmiTrigger/conv /Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug /Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug /Users/mari/work/MPPCinstall/cosmiTrigger/conv/cmake-build-debug/CMakeFiles/conv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/conv.dir/depend

