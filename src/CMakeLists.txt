## CMake settings

cmake_minimum_required (VERSION 2.6)
project(MTE7101)

# Enable verbosity
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set compiler to GCC/G++
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
# Use C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# Do not show deprecated warnings
# add_definitions("-Wno-deprecated")

# Output various settings
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Use package PkgConfig
find_package(PkgConfig REQUIRED)

# Use package OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    message(STATUS "Found OpenGL: ${OPENGL_LIBRARY}" )
endif(OPENGL_FOUND)

# Use package GLEW
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    message(STATUS "Found GLEW: ${GLEW_LIBRARY}" )
endif(GLEW_FOUND)

# Use package GLFW3
if(APPLE)
    find_package(GLFW3 REQUIRED)
    if(GLFW3_FOUND)
        message(STATUS "Found GLFW: ${GLFW3_LIBRARY}")
    endif(GLFW3_FOUND)
else
    pkg_search_module(GLFW REQUIRED glfw3)
    if(GLFW_FOUND)
        message(STATUS "Found GLFW: ${GLFW3_LIBRARY}")
    endif(GLFW_FOUND)
endif()

# Specify additional include directories
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/../inc)

# Set source files
set(SOURCES
    fatal_exception.hpp
    main.cpp
    main.hpp
    shader.cpp
    shader.hpp
    shader_factory.cpp
    shader_factory.hpp
)

# Specify additional linking directories
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Specify executable file for output
add_executable(mte7101 ${SOURCES})

# Link needed libraries
target_link_libraries(mte7101
    ${OPENGL_LIBRARY}
    ${GLFW3_LIBRARY}
    ${GLU_LIBRARY}
    ${EXTRA_LIBS}
    ${GLEW_LIBRARY}
    ${EXTRA_LIBS}
    boost_system
    boost_filesystem
    boost_regex
)

# Set output directory of executable
set_target_properties(mte7101 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin
)
