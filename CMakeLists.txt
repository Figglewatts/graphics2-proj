cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(graphics2_proj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(include)

file(GLOB_RECURSE SOURCE_FILES
        src/*.c
        src/*.cpp
        src/*.h)

add_executable(graphics2_proj ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLFW_INCLUDE_DIRS} )

target_link_libraries(graphics2_proj ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES} )