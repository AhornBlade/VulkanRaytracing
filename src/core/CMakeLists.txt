add_library(VulkanRenderer Instance.cpp "Module.cpp" "Presention.cpp"  "Swapchain.cpp")
target_link_libraries(VulkanRenderer 
	PUBLIC Vulkan::Headers)
target_include_directories(VulkanRenderer
	INTERFACE .)