cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

project (raytracing)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIRECTORY src)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${SOURCE_DIRECTORY}/*.cpp ${SOURCE_DIRECTORY}/*.h)

add_executable(app ${SOURCE_FILES})
target_include_directories(app PRIVATE ${SOURCE_DIRECTORY})
