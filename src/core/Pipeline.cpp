#include "Pipeline.hpp"

Pipeline::Pipeline(Pipeline&& other) noexcept
	: Module{ std::move(other) }, pipelineCache{ std::move(other.pipelineCache) },
	pipeline{ std::move(other.pipeline) } {}

Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
{
	Module::operator=(std::move(other));
	std::swap(other.pipelineCache, pipelineCache);
	std::swap(other.pipeline, pipeline);
	return *this;
}

void Pipeline::createPipelineCache()
{
	vk::PipelineCacheCreateInfo createInfo{};

	pipelineCache = vk::raii::PipelineCache{ device, createInfo };
}
