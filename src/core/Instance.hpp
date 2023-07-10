#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <vector>

class Instance
{
public:
	Instance() = default;
	Instance(
		const std::vector<const char*>& enabledExtensions,
		const std::vector<const char*>& enabledLayers);

	template<class ModuleType, class ... Args>
	ModuleType createModule(Args&&... args)const
	{
		return ModuleType{ *this, (std::forward<Args>args)... };
	}

	inline const auto& getPhysicalDevices()const noexcept
	{
		return physicalDevices;
	}

	inline const auto& getInstance() const noexcept
	{
		return instance;
	}

private:
	vk::raii::Context context;
	vk::raii::Instance instance{nullptr};
	vk::raii::DebugUtilsMessengerEXT debugMessenger{nullptr};
	vk::raii::PhysicalDevices physicalDevices{nullptr};

	void createInsance(
		const std::vector<const char*>& extensions,
		const std::vector<const char*>& layers);
	
};