# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.4.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.4.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/build

# Include any dependencies generated for this target.
include CMakeFiles/Elastic2D.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Elastic2D.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Elastic2D.dir/flags.make

CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o: CMakeFiles/Elastic2D.dir/flags.make
CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o: ../Elastic2D.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o -c /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/Elastic2D.cpp

CMakeFiles/Elastic2D.dir/Elastic2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Elastic2D.dir/Elastic2D.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/Elastic2D.cpp > CMakeFiles/Elastic2D.dir/Elastic2D.cpp.i

CMakeFiles/Elastic2D.dir/Elastic2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Elastic2D.dir/Elastic2D.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/Elastic2D.cpp -o CMakeFiles/Elastic2D.dir/Elastic2D.cpp.s

CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o.requires:

.PHONY : CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o.requires

CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o.provides: CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o.requires
	$(MAKE) -f CMakeFiles/Elastic2D.dir/build.make CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o.provides.build
.PHONY : CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o.provides

CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o.provides.build: CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o


CMakeFiles/Elastic2D.dir/main.cpp.o: CMakeFiles/Elastic2D.dir/flags.make
CMakeFiles/Elastic2D.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Elastic2D.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Elastic2D.dir/main.cpp.o -c /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/main.cpp

CMakeFiles/Elastic2D.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Elastic2D.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/main.cpp > CMakeFiles/Elastic2D.dir/main.cpp.i

CMakeFiles/Elastic2D.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Elastic2D.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/main.cpp -o CMakeFiles/Elastic2D.dir/main.cpp.s

CMakeFiles/Elastic2D.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Elastic2D.dir/main.cpp.o.requires

CMakeFiles/Elastic2D.dir/main.cpp.o.provides: CMakeFiles/Elastic2D.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Elastic2D.dir/build.make CMakeFiles/Elastic2D.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Elastic2D.dir/main.cpp.o.provides

CMakeFiles/Elastic2D.dir/main.cpp.o.provides.build: CMakeFiles/Elastic2D.dir/main.cpp.o


# Object files for target Elastic2D
Elastic2D_OBJECTS = \
"CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o" \
"CMakeFiles/Elastic2D.dir/main.cpp.o"

# External object files for target Elastic2D
Elastic2D_EXTERNAL_OBJECTS =

bin/Elastic2D: CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o
bin/Elastic2D: CMakeFiles/Elastic2D.dir/main.cpp.o
bin/Elastic2D: CMakeFiles/Elastic2D.dir/build.make
bin/Elastic2D: /Users/bradleybeth/src/installs/simit/build/lib/libsimit.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMBitWriter.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMipo.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMVectorize.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMX86Disassembler.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMX86AsmParser.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMX86CodeGen.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMSelectionDAG.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMAsmPrinter.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMMCParser.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMX86Desc.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMX86Info.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMX86AsmPrinter.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMX86Utils.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMJIT.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMRuntimeDyld.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMExecutionEngine.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMCodeGen.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMObjCARCOpts.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMScalarOpts.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMInstCombine.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMTransformUtils.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMipa.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMAnalysis.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMTarget.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMMC.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMObject.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMCore.a
bin/Elastic2D: /usr/local/Cellar/llvm34/3.4.2/lib/llvm-3.4/lib/libLLVMSupport.a
bin/Elastic2D: CMakeFiles/Elastic2D.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable bin/Elastic2D"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Elastic2D.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Elastic2D.dir/build: bin/Elastic2D

.PHONY : CMakeFiles/Elastic2D.dir/build

CMakeFiles/Elastic2D.dir/requires: CMakeFiles/Elastic2D.dir/Elastic2D.cpp.o.requires
CMakeFiles/Elastic2D.dir/requires: CMakeFiles/Elastic2D.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Elastic2D.dir/requires

CMakeFiles/Elastic2D.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Elastic2D.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Elastic2D.dir/clean

CMakeFiles/Elastic2D.dir/depend:
	cd /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/build /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/build /Users/bradleybeth/src/git/simit-bgb/Elastic2D/simit/build/CMakeFiles/Elastic2D.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Elastic2D.dir/depend

