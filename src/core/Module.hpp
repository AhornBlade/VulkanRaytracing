#pragma once

#include "Instance.hpp"

class Module
{
public:
	virtual ~Module() = default;

	static std::vector<const char*> getInstanceExtensions();
	static std::vector<const char*> getInstanceLayers();

protected:
	Module() = default;

	vk::raii::Device device{nullptr};
	std::vector<vk::raii::Queue> queues;

	uint32_t getQueueFamilyIndex(
		const vk::raii::PhysicalDevice& physicalDevice,
		uint32_t queueCount,
		vk::QueueFlags queueType)const noexcept;

	void createDevice(
		const vk::raii::PhysicalDevice& physicalDevice,
		uint32_t queueFamilyIndex,
		uint32_t queueCount,
		const std::vector<const char*>& extensions = {},
		const std::vector<const char*>& layers = {},
		const vk::PhysicalDeviceFeatures& features = {},
		float priorities = 0.5f);
};