#include "Module.hpp"

#include <iostream>

Module& Module::operator=(Module&& other) noexcept
{
	if (this == &other)
		return *this;
	std::swap(other.device, device);
	std::swap(other.queues, queues);
	return *this;
}

std::vector<const char*>& Module::getInstanceExtensions(std::vector<const char*>& extensions) noexcept
{
	return extensions;
}
std::vector<const char*>& Module::getInstanceLayers(std::vector<const char*>& layers) noexcept
{
	return layers;
}

Module::Module(
	const vk::raii::PhysicalDevice& physicalDevice,
	uint32_t queueFamilyIndex,
	uint32_t queueCount,
	const std::vector<const char*>& extensions = {},
	const std::vector<const char*>& layers = {},
	const vk::PhysicalDeviceFeatures& features = {},
	float priorities = 0.5f)
{
	createDevice(physicalDevice, queueFamilyIndex, queueCount, extensions, layers, features, priorities);
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
	vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
	std::cout << "Info: Using Physical Device: " << physicalDeviceProperties.deviceName << "\n";

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
		queues.emplace_back(device.getQueue(queueFamilyIndex, index));
	}

	std::cout << "Info: Success to create " << queues.size() << " queues\n";
}

