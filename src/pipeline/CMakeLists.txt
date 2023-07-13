add_library(VulkanPipeline GraphicsPipeline.cpp)

target_link_libraries(VulkanPipeline
	PUBLIC VulkanRenderer)

