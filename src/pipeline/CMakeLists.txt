add_library(VulkanPipeline GraphicsPipeline.cpp "PipelineCreateInfo.cpp")

target_link_libraries(VulkanPipeline
	PUBLIC VulkanRenderer)

