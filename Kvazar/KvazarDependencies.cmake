add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/spdlog)
add_subdirectory(3rdparty/vk-bootstrap)
add_subdirectory(3rdparty/VMA)

target_link_libraries(Kvazar PUBLIC 
	glfw 
	spdlog 
	vk-bootstrap::vk-bootstrap
	GPUOpen::VulkanMemoryAllocator)