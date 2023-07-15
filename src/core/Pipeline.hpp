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
		const CreateInfo& pipelineCreateInfo)
		: Module{physicalDevice, queueFamilyIndex, queueCount, pipelineCreateInfo.extensions }
	{
		createPipelineCache();
		pipelineCreateInfo.initialize(device);
		pipeline = vk::raii::Pipeline{ device, pipelineCache, pipelineCreateInfo };
	}

	Pipeline(Pipeline&& other) noexcept;
	Pipeline& operator=(Pipeline&& other) noexcept;

protected:
	vk::raii::PipelineCache pipelineCache{ nullptr };
	vk::raii::Pipeline pipeline{ nullptr };

	void createPipelineCache();

};