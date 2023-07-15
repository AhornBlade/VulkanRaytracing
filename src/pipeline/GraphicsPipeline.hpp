#pragma once

#include "PipelineCreateInfo.hpp"

#include <optional>

struct SubpassDescription
{
	vk::PipelineBindPoint pipelineBindPoint;
	std::vector<vk::AttachmentReference> inputAttachments;
	std::vector<vk::AttachmentReference> colorAttachments;
	std::vector<vk::AttachmentReference> resolveAttachments;
	std::vector<vk::AttachmentReference> depthStencilAttachments;
	std::vector<uint32_t> preserveAttachments;

	operator vk::SubpassDescription() const;
};

struct GraphicsPipelineCreateInfo: public PipelineCreateInfo
{
	operator vk::GraphicsPipelineCreateInfo ()const;

	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
	vk::PipelineTessellationStateCreateInfo tessellationState;
	vk::PipelineVertexInputStateCreateInfo vertexInputState;
	vk::PipelineRasterizationStateCreateInfo rasterizationState;
	std::vector<vk::PipelineColorBlendAttachmentState> blendAttachmentStates;
	std::optional<vk::LogicOp> colorBlendLogicOp;
	vk::PipelineDepthStencilStateCreateInfo depthStencilState;
	std::vector<vk::Viewport> viewports;
	std::vector<vk::Rect2D> scissors;
	vk::PipelineMultisampleStateCreateInfo multisampleState;
	std::vector<vk::DynamicState> dynamicStates;
	std::vector<vk::AttachmentDescription> attachments;
	std::vector<vk::SubpassDependency> subpassDependencies;
	std::vector<SubpassDescription> subpassDescriptions;

	void initialize(const vk::raii::Device& device) const;

private:
	mutable vk::PipelineViewportStateCreateInfo viewportState;
	mutable vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
	mutable vk::PipelineColorBlendStateCreateInfo colorBlendState;
	mutable vk::raii::RenderPass renderpass{ nullptr };

	void createRenderPass(const vk::raii::Device& device) const;
};