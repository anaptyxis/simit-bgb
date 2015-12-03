cmake_minimum_required(VERSION 2.6)

project(SpringSystem)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#set(SimitSrcDir "" CACHE PATH "Choosen By User")
if (DEFINED ENV{SIMIT_SRC_DIR})
  set(SIMIT_SRC_DIR $ENV{SIMIT_SRC_DIR})
else ()
  set(SIMIT_SRC_DIR "" CACHE PATH "Choosen By User")
endif()

if (DEFINED ENV{SIMIT_LIB})
  set(SIMIT_LIB $ENV{SIMIT_LIB})
else ()
  set(SIMIT_LIB "" CACHE FILEPATH "Choosen By User")
endif ()

if (DEFINED ENV{SIMIT_FLOAT_TYPE})
  set(SIMIT_FLOAT_TYPE $ENV{SIMIT_FLOAT_TYPE})
else ()
  set(SIMIT_FLOAT_TYPE "double")
endif()

if (NOT CMAKE_BUILD_TYPE MATCHES Release)
  add_definitions(-DPRINT)
endif ()

include_directories("/usr/local/include")
include_directories(${SIMIT_SRC_DIR})
include_directories(${SpringSystem_INCLUDE_DIR} ${SpringSystem_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW3_INCLUDE_DIR})

file(GLOB SPRINGSYSTEM_HEADER_CODE ${SpringSystem_SOURCE_DIR}/*.h)
file(GLOB SPRINGSYSTEM_SOURCE_CODE ${SpringSystem_SOURCE_DIR}/*.cpp)
file(GLOB SPRINGSYSTEM_SIMIT_CODE  ${SpringSystem_SOURCE_DIR}/*.sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Dsimit_float=${SIMIT_FLOAT_TYPE}")

add_definitions(-DSIMIT_CODE_DIR=${SpringSystem_SOURCE_DIR})

add_executable(SpringSystem ${SPRINGSYSTEM_HEADER_CODE} ${SPRINGSYSTEM_SOURCE_CODE} ${SPRINGSYSTEM_SIMIT_CODE} ${SpringSystem_SOURCE_DIR})
link_libraries(${GLFW_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} ${SIMIT_LIB})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} ${GLFW_STATIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "-L/usr/local/Cellar/glfw3/3.1.2/lib")

IF(APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(GLUT_LIBRARY GLUT )
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
   MARK_AS_ADVANCED (COCOA_LIBRARY
                     GLUT_LIBRARY
                     OpenGL_LIBRARY)
   SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
ENDIF (APPLE)

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
#target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

message(STATUS ${EXTRA_LIBS})
message(STATUS ${GLFW_LIBRARIES} )
message(STATUS ${GLFW_STATIC_LIBRARIES} )
message(STATUS ${GLFW_LIBRARY_DIRS})
message("-----------")

# LLVM
if (DEFINED ENV{LLVM_CONFIG})
  set(LLVM_CONFIG $ENV{LLVM_CONFIG})
else()
  set(LLVM_CONFIG "llvm-config")
endif()

execute_process(COMMAND ${LLVM_CONFIG} --version RESULT_VARIABLE LLVM_CONFIG_RESULT OUTPUT_VARIABLE LLVM_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT ${LLVM_CONFIG_RESULT} STREQUAL "0")
  if (DEFINED ENV{LLVM_CONFIG})
    message(FATAL_ERROR "Could not execute llvm-config (LLVM_CONFIG=${LLVM_CONFIG}). Add llvm-config version 3.3 or higher to PATH or set the LLVM_CONFIG environment variable to point to it.")
  else()
    message(FATAL_ERROR "Could not execute llvm-config. Add llvm-config version 3.3 or higher to PATH or set the LLVM_CONFIG environment variable to point to it.")
  endif()
endif()

if (NOT ${LLVM_VERSION} MATCHES "^([3-9]|[0-9][0-9]+)\\.([3-9]|[0-9][0-9]+)(\\.[0-9]+)?(svn)?$")
  message(FATAL_ERROR "Need LLVM version 3.3 or higher. Found ${LLVM_CONFIG} version \"${LLVM_VERSION}\".")
elseif (${LLVM_VERSION} MATCHES "^([3-9]|[0-9][0-9]+)\\.([5-9]|[0-9][0-9]+)(\\.[0-9]+)?(svn)?$")
  set(LLVM_CONFIG_GT_45 1)
endif()

message("-- Found LLVM: ${LLVM_CONFIG} (found version \"${LLVM_VERSION}\")")

execute_process(COMMAND ${LLVM_CONFIG} --cppflags OUTPUT_VARIABLE LLVM_CPPFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_FLAGS ${LLVM_CPPFLAGS})

execute_process(COMMAND ${LLVM_CONFIG} --includedir OUTPUT_VARIABLE LLVM_INCLUDES OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories("${LLVM_INCLUDES}")

execute_process(COMMAND ${LLVM_CONFIG} --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
if (DEFINED LLVM_CONFIG_GT_45)
  execute_process(COMMAND ${LLVM_CONFIG} --system-libs OUTPUT_VARIABLE LLVM_SYSTEM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(LLVM_LDFLAGS ${LLVM_LDFLAGS} ${LLVM_SYSTEM_LIBS})
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY LINK_FLAGS ${LLVM_LDFLAGS})

if ($ENV{SIMIT_NO_IPO})
  message("building without ipo")
  set(LLVM_COMPONENTS core jit x86 vectorize bitwriter)
else ()
  set(LLVM_COMPONENTS core jit x86 ipo vectorize bitwriter)
endif()

execute_process(COMMAND ${LLVM_CONFIG} --libnames ${LLVM_COMPONENTS} OUTPUT_VARIABLE LLVM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
separate_arguments(LLVM_LIBS)  # Turn the space-separate string into a ;-separated list
execute_process(COMMAND ${LLVM_CONFIG} --libdir OUTPUT_VARIABLE LLVM_LIBDIR OUTPUT_STRIP_TRAILING_WHITESPACE)

foreach(lib ${LLVM_LIBS})
  find_library(libpath NAMES ${lib} NO_DEFAULT_PATH PATHS ${LLVM_LIBDIR} PATH_SUFFIXES Debug Release MinSizeRel RelWithDebInfo)
  target_link_libraries(${PROJECT_NAME} ${libpath})
  unset(libpath CACHE)
endforeach()

separate_arguments(LLVM_LDFLAGS)
foreach(ldflag ${LLVM_LDFLAGS})
  string(STRIP ${ldflag} ldflag)
  if (${ldflag} MATCHES "^-l.*$")
	 string(LENGTH ${ldflag} len)
	 MATH(EXPR len ${len}-2)
	 string(SUBSTRING ${ldflag} 2 ${len} ldflag)
	 target_link_libraries(${PROJECT_NAME} ${ldflag})
  endif()
endforeach()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(DARWIN TRUE)
endif()
# CUDA
if (DEFINED ENV{SIMIT_CUDA_ENABLE})
  find_package(CUDA)
  if (CUDA_FOUND)
    message("-- Building with gpu-backend")
    # Linking
    if (LINUX)
      find_library(libcuda NAMES cuda PATHS "${CUDA_TOOLKIT_ROOT_DIR}/lib64")
      find_library(libnvvm NAMES nvvm PATHS "${CUDA_TOOLKIT_ROOT_DIR}/nvvm/lib64")
      target_link_libraries(${PROJECT_NAME} ${libcuda})
      target_link_libraries(${PROJECT_NAME} ${libnvvm})
      # Profiling hack
      find_library(libcudart NAMES cudart PATHS "${CUDA_TOOLKIT_ROOT_DIR}/lib64")
      target_link_libraries(${PROJECT_NAME} ${libcudart})
    elseif (DARWIN)
      target_link_libraries(${PROJECT_NAME} ${CUDA_LIBRARIES})
      target_link_libraries(${PROJECT_NAME} "${CUDA_TOOLKIT_ROOT_DIR}/lib/libcuda.dylib")
      target_link_libraries(${PROJECT_NAME} "${CUDA_TOOLKIT_ROOT_DIR}/nvvm/lib/libnvvm.dylib")
    endif ()
  else ()
    message("-- Building without gpu-backend")
  endif ()
endif ()

