cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Kvazar)

file(GLOB_RECURSE sourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE includeFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sourceFiles} ${includeFiles})


# Kvazar target
add_library(Kvazar STATIC ${sourceFiles} ${includeFiles})

target_precompile_headers(Kvazar PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/KvazarPch.h)

target_compile_definitions(Kvazar PRIVATE $<$<CONFIG:Debug>:KVAZAR_BUILD_DEBUG>)

target_include_directories(Kvazar PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Platform
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
)


# Vulkan integration
find_package(Vulkan REQUIRED) 
if(NOT Vulkan_FOUND)
	message(FATAL_ERROR "\t>>> Vulkan package not found <<<")
else()
	message(STATUS "\t\t>>> Vulkan package was found, keep going ... <<<")
	message(STATUS "\t\t>>> We got libs from package: ${Vulkan_LIBRARIES} <<<")
endif()
target_link_libraries(Kvazar PRIVATE Vulkan::Vulkan)


include(KvazarDependencies.cmake)