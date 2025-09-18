cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Kvazar)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options(/W4)
endif()

# Engine side
add_subdirectory(Kvazar)

# Client side
add_subdirectory(Client)

# properties
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Client)