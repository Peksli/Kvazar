cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Client)

file(GLOB_RECURSE sourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE includeFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sourceFiles} ${includeFiles})


# Client target
add_executable(Client ${sourceFiles} ${includeFiles})

target_link_libraries(Client PUBLIC Kvazar)