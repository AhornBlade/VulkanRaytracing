#pragma once

#include "Module.hpp"

class Pipeline: public Module
{
public:
	Pipeline() = default;
	template<class CreateInfo>
	Pipeline(
		const vk::raii::PhysicalDevice& physicalDevice,
		uint32_t queueFamilyIndex,
		uint32_t queueCount,
		const CreateInfo& pipelineCreateInfo,
		const std::vector<vk::DescriptorSetLayout>& setLayouts,
		const std::vector<vk::PushConstantRange>& pushConstantRanges)
		: Module{physicalDevice, queueFamilyIndex, queueCount}
	{
		createPipelineLayout(setLayouts, pushConstantRanges);
		pipelineCreateInfo.setPipelineLayout(pipelineLayout);
		createPipelineCache();
		createPipeline(pipelineCreateInfo);
	}

	Pipeline(Pipeline&& other) noexcept;
	Pipeline& operator=(Pipeline&& other) noexcept;

protected:
	vk::raii::PipelineLayout pipelineLayout{ nullptr };
	vk::raii::PipelineCache pipelineCache{ nullptr };
	vk::raii::Pipeline pipeline{ nullptr };

	void createPipelineLayout(
		const std::vector<vk::DescriptorSetLayout>& setLayouts,
		const std::vector<vk::PushConstantRange>& pushConstantRanges);
	void createPipelineCache();

	template<class CreateInfo>
	void createPipeline(const CreateInfo& createInfo)
	{
		pipeline = vk::raii::Pipeline{device, pipelineCache, createInfo};
	}

};