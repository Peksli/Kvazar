add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/spdlog)

target_link_libraries(Kvazar PUBLIC glfw spdlog)