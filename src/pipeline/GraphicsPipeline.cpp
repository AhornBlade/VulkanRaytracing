#include "GraphicsPipeline.hpp"

GraphicsPipelineCreateInfo::operator vk::GraphicsPipelineCreateInfo ()const
{
	vk::GraphicsPipelineCreateInfo pipelineCreateInfo{};

	pipelineCreateInfo.setPInputAssemblyState(&inputAssemblyState);
	pipelineCreateInfo.setPTessellationState(&tessellationState);
	pipelineCreateInfo.setPVertexInputState(&vertexInputState);
	pipelineCreateInfo.setPRasterizationState(&rasterizationState);
	colorBlendState.setAttachments(blendAttachmentStates);
	if (colorBlendLogicOp.has_value())
	{
		colorBlendState.setLogicOpEnable(VK_TRUE);
		colorBlendState.setLogicOp(colorBlendLogicOp.value());
	}
	pipelineCreateInfo.setPColorBlendState(&colorBlendState);
	pipelineCreateInfo.setPDepthStencilState(&depthStencilState);
	viewportState.setViewports(viewports);
	viewportState.setScissors(scissors);
	pipelineCreateInfo.setPViewportState(&viewportState);
	pipelineCreateInfo.setPMultisampleState(&multisampleState);
	dynamicStateInfo.setDynamicStates(dynamicStates);
	pipelineCreateInfo.setPDynamicState(&dynamicStateInfo);

	pipelineCreateInfo.setStages(shaderStageCreateInfos);
	pipelineCreateInfo.setLayout(*pipelineLayout);
	pipelineCreateInfo.setRenderPass(*renderpass);

	return pipelineCreateInfo;
}

void GraphicsPipelineCreateInfo::initialize(const vk::raii::Device& device) const
{
	PipelineCreateInfo::initialize(device);
	createRenderPass(device);
}

void GraphicsPipelineCreateInfo::createRenderPass(const vk::raii::Device& device) const
{
	std::vector<vk::SubpassDescription> descriptions;

	for (auto& subpassDescription : subpassDescriptions)
	{
		descriptions.emplace_back(subpassDescription);
	}

	vk::RenderPassCreateInfo createInfo;
	createInfo.setAttachments(attachments);
	createInfo.setDependencies(subpassDependencies);
	createInfo.setSubpasses(descriptions);

	renderpass = vk::raii::RenderPass{ device, createInfo };
}

SubpassDescription::operator vk::SubpassDescription() const
{
	vk::SubpassDescription description;
	description.setPipelineBindPoint(pipelineBindPoint);
	description.setInputAttachments(inputAttachments);
	description.setColorAttachments(colorAttachments);
	description.setResolveAttachments(resolveAttachments);
	description.setPDepthStencilAttachment(depthStencilAttachments.data());
	description.setPreserveAttachments(preserveAttachments);

	return description;
}
