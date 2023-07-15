#pragma once

#include "Module.hpp"
#include "Swapchain.hpp"

class Window: public Module
{
public:
	Window(
		const vk::raii::PhysicalDevice& physicalDevice, 
		uint32_t queueFamilyIndex, 
		vk::raii::SurfaceKHR&& _surface);

	template<class SurfaceCreateInfo>
	Window(
		const Instance& instance,
		const vk::raii::PhysicalDevice& physicalDevice,
		uint32_t queueFamilyIndex,
		const SurfaceCreateInfo& surfaceCreateInfo)
		: Window{ physicalDevice, queueFamilyIndex,
		vk::raii::SurfaceKHR{instance.getInstance(), surfaceCreateInfo}} {}

	Window(Window&& other) noexcept;
	Window& operator=(Window&& other) noexcept;

	static std::vector<const char*>& getInstanceExtensions(std::vector<const char*>& extensions) noexcept;

	vk::Extent2D getImageExtent() const noexcept
	{
		return swapchainCreateInfo.imageExtent;
	}

	vk::Format getImageFormat()const noexcept
	{
		return swapchainCreateInfo.imageFormat;
	}

	void rebuildSwapchain();

protected:
	Window() = default;

	vk::raii::PhysicalDevice physicalDevice{nullptr};
	vk::raii::SurfaceKHR surface{ nullptr };
	vk::SwapchainCreateInfoKHR swapchainCreateInfo;
	Swapchain swapchain{ nullptr };

	void initialSwapchainCreateInfo();
	void updateSwapchainCreateInfo();
};