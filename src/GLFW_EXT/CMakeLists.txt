add_library(VulkanGLFW_EXT GLFWWindow.cpp)

target_link_libraries(VulkanGLFW_EXT
	PUBLIC VulkanRenderer
	PUBLIC glfw)

target_include_directories(VulkanGLFW_EXT
	INTERFACE .
)