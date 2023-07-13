#include "GraphicsPipeline.hpp"

GraphicsPipelineCreateInfo::operator vk::GraphicsPipelineCreateInfo ()
{
	vk::GraphicsPipelineCreateInfo pipelineCreateInfo{};

	pipelineCreateInfo.setPInputAssemblyState(&inputAssemblyState);
	pipelineCreateInfo.setPTessellationState(&tessellationState);
	pipelineCreateInfo.setPVertexInputState(&vertexInputState);
	pipelineCreateInfo.setPRasterizationState(&rasterizationState);
	colorBlendState.setAttachments(blendAttachmentStates);
	pipelineCreateInfo.setPColorBlendState(&colorBlendState);
	pipelineCreateInfo.setPDepthStencilState(&depthStencilState);
	viewportState.setViewports(viewports);
	viewportState.setScissors(scissors);
	pipelineCreateInfo.setPViewportState(&viewportState);
	pipelineCreateInfo.setPMultisampleState(&multisampleState);
	dynamicStateInfo.setDynamicStates(dynamicStates);
	pipelineCreateInfo.setPDynamicState(&dynamicStateInfo);
	pipelineCreateInfo.setStages(shaderStages);

	pipelineCreateInfo.setLayout(pipelineLayout);

	return pipelineCreateInfo;
}
