#include "Module.hpp"

#include <iostream>

std::vector<const char*> Module::getInstanceExtensions()
{
	return {};
}

std::vector<const char*> Module::getInstanceLayers()
{
	return {};
}

uint32_t Module::getQueueFamilyIndex(const vk::raii::PhysicalDevice& physicalDevice, uint32_t queueCount, vk::QueueFlags queueType) const noexcept
{
	auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
	for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyProperties.size(); queueFamilyIndex++)
	{
		auto property = queueFamilyProperties[queueFamilyIndex];
		if ((property.queueCount >= queueCount) &&
			((property.queueFlags & queueType) == queueType))
		{
			return queueFamilyIndex;
		}
	}

	std::cout << "Warning: Failed to find suitable queue family\n";
	return queueFamilyProperties.size();
}

void Module::createDevice(
	const vk::raii::PhysicalDevice& physicalDevice, 
	uint32_t queueFamilyIndex, 
	uint32_t queueCount, 
	const std::vector<const char*>& extensions, 
	const std::vector<const char*>& layers, 
	const vk::PhysicalDeviceFeatures& features, 
	float priorities)
{
	std::vector<float> queuePriorities(queueCount, priorities);
	vk::DeviceQueueCreateInfo queueCreateInfo;
	queueCreateInfo.setQueueFamilyIndex(queueFamilyIndex);
	queueCreateInfo.setQueuePriorities(queuePriorities);

	vk::DeviceCreateInfo createInfo;
	createInfo.setPEnabledExtensionNames(extensions);
#ifdef VULKAN_RAY_TRACING_DEBUG
	std::vector<const char*> enabledLayers = layers;
	enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
	createInfo.setPEnabledLayerNames(enabledLayers);
#else
	createInfo.setPEnabledLayerNames(layers);
#endif
	createInfo.setPEnabledFeatures(&features);
	createInfo.setQueueCreateInfos(queueCreateInfo);

	device = physicalDevice.createDevice(createInfo);

	std::cout << "Info: Success to create device\n";

	for (uint32_t index = 0; index < queueCount; index++)
	{
		queues.emplace_back(device.getQueue(queueFamilyIndex, queueCount));
	}

	std::cout << "Info: Success to create " << queues.size() << " queues\n";
}

