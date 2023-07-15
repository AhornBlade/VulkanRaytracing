#pragma once

#include "Instance.hpp"

class Module
{
public:
	virtual ~Module() noexcept = default;

	Module(Module&& other) noexcept : device{ std::move(other.device) }, queues{ std::move(other.queues) } {}
	Module& operator=(Module&& other) noexcept;

	static std::vector<const char*>& getInstanceExtensions(std::vector<const char*>& extensions) noexcept;
	static std::vector<const char*>& getInstanceLayers(std::vector<const char*>& layers) noexcept;

	const vk::raii::Device& getDevice() const noexcept
	{
		return device;
	}

protected:
	Module() = default;
	Module(
		const vk::raii::PhysicalDevice& physicalDevice,
		uint32_t queueFamilyIndex,
		uint32_t queueCount,
		const std::vector<const char*>& extensions = {},
		const std::vector<const char*>& layers = {},
		const vk::PhysicalDeviceFeatures& features = {},
		float priorities = 0.5f);

	vk::raii::Device device{nullptr};
	std::vector<vk::raii::Queue> queues;

	void createDevice(
		const vk::raii::PhysicalDevice& physicalDevice,
		uint32_t queueFamilyIndex,
		uint32_t queueCount,
		const std::vector<const char*>& extensions = {},
		const std::vector<const char*>& layers = {},
		const vk::PhysicalDeviceFeatures& features = {},
		float priorities = 0.5f);
};

template<class First, class ... ModuleTypes>
std::vector<const char*>& getInstanceExtensionsImpl(std::vector<const char*>& extensions) noexcept
{
	if constexpr(sizeof...(ModuleTypes) == 0)
		return First::getInstanceExtensions(extensions);
	else
		return getInstanceExtensionsImpl<ModuleTypes...>(First::getInstanceExtensions(extensions));
}

template<class First, class ... ModuleTypes>
std::vector<const char*>& getInstanceLayersImpl(std::vector<const char*>& layers) noexcept
{
	if constexpr(sizeof...(ModuleTypes) == 0)
		return First::getInstanceLayers(layers);
	else
		return getInstanceLayersImpl<ModuleTypes...>(First::getInstanceLayers(layers));
}

template<class ... ModuleTypes>
std::vector<const char*> getInstanceExtensions() noexcept
{
	std::vector<const char*> extensions;
	return getInstanceExtensionsImpl<ModuleTypes...>(extensions);
}

template<class ... ModuleTypes>
std::vector<const char*> getInstanceLayers() noexcept
{
	std::vector<const char*> layers;
	return getInstanceLayersImpl<ModuleTypes...>(layers);
}