#pragma once

#include <vulkan/vulkan.hpp>

struct GraphicsPipelineCreateInfo
{
	operator vk::GraphicsPipelineCreateInfo ();

	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
	vk::PipelineTessellationStateCreateInfo tessellationState;
	vk::PipelineVertexInputStateCreateInfo vertexInputState;
	vk::PipelineRasterizationStateCreateInfo rasterizationState;
	std::vector<vk::PipelineColorBlendAttachmentState> blendAttachmentStates;
	vk::PipelineColorBlendStateCreateInfo colorBlendState;
	vk::PipelineDepthStencilStateCreateInfo depthStencilState;
	std::vector<vk::Viewport> viewports;
	std::vector<vk::Rect2D> scissors;
	vk::PipelineMultisampleStateCreateInfo multisampleState;
	std::vector<vk::DynamicState> dynamicStates;
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
	void setPipelineLayout(const vk::PipelineLayout& layout)
	{
		pipelineLayout = layout;
	}

private:
	vk::PipelineViewportStateCreateInfo viewportState;
	vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
	vk::PipelineLayout pipelineLayout;
};