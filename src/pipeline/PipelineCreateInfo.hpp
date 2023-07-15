#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <vector>

struct ShaderCreateInfo
{
	std::vector<unsigned char> code;
	vk::ShaderStageFlagBits shaderStage;
};

struct PipelineCreateInfo
{
public:

	void initialize(const vk::raii::Device& device)const;

	std::vector<const char*> extensions;
	std::vector<vk::PushConstantRange> pushConstantRanges;
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
	std::vector<ShaderCreateInfo> shaderCreateInfos;

protected:
	mutable vk::raii::PipelineLayout pipelineLayout{ nullptr };
	mutable std::vector<vk::raii::ShaderModule> shaderModules;
	mutable std::vector<vk::SpecializationInfo> specializationInfos;
	mutable std::vector<vk::PipelineShaderStageCreateInfo> shaderStageCreateInfos;

	void createPipelineLayout(const vk::raii::Device& device)const;
	void createShaderStageCreateInfos(const vk::raii::Device& device)const;
};