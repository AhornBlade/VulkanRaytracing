add_subdirectory(core)
add_subdirectory(GLFW_EXT)
add_subdirectory(pipeline)

add_library(VulkanRaytracing INTERFACE)
target_link_libraries(VulkanRaytracing
	INTERFACE VulkanRenderer
	INTERFACE VulkanGLFW_EXT
	INTERFACE VulkanPipeline)
target_include_directories(VulkanRaytracing
	INTERFACE .)