#include "PipelineCreateInfo.hpp"

void PipelineCreateInfo::initialize(const vk::raii::Device& device) const
{
	createPipelineLayout(device);
	createShaderStageCreateInfos(device);
}

void PipelineCreateInfo::createPipelineLayout(const vk::raii::Device& device) const
{
	vk::PipelineLayoutCreateInfo createInfo;
	createInfo.setPushConstantRanges(pushConstantRanges);
	createInfo.setSetLayouts(descriptorSetLayouts);

	pipelineLayout = vk::raii::PipelineLayout{ device, createInfo };
}

void PipelineCreateInfo::createShaderStageCreateInfos(const vk::raii::Device& device) const
{
	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{};
	vk::PipelineShaderStageCreateInfo stageCreateInfo{};

	for (auto& shaderCreateInfo : shaderCreateInfos)
	{
		shaderModuleCreateInfo.setPCode(reinterpret_cast<const uint32_t*>(shaderCreateInfo.code.data()));
		shaderModuleCreateInfo.setCodeSize(shaderCreateInfo.code.size());
		shaderModules.emplace_back(vk::raii::ShaderModule{device, shaderModuleCreateInfo});
		stageCreateInfo.setModule(*(shaderModules.back()));
		stageCreateInfo.setPName("main");
		stageCreateInfo.setStage(shaderCreateInfo.shaderStage);
		shaderStageCreateInfos.push_back(stageCreateInfo);
	}
}
