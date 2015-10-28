cmake_minimum_required (VERSION 2.6)
project(MTE7101)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions("-Wno-deprecated")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    message(STATUS "Found OpenGL: ${OPENGL_LIBRARY}" )
endif(OPENGL_FOUND)
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    message(STATUS "Found GLEW: ${GLEW_LIBRARY}" )
endif(GLEW_FOUND)
pkg_search_module(GLFW REQUIRED glfw3)
if(GLFW_FOUND)
    message(STATUS "Found GLFW: ${GLFW_LIBRARIES}")
endif(GLFW_FOUND)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    main.cpp
    main.hpp
    fatal_exception.hpp
)
link_directories(${CMAKE_SOURCE_DIR}/lib)
add_executable(mte7101 ${SOURCES})

target_link_libraries(mte7101
    ${GLFW_STATIC_LIBRARIES}
    ${GLU_LIBRARY}
    ${EXTRA_LIBS}
    ${GLEW_LIBRARY}
)

set_target_properties(mte7101 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin
)
