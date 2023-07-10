#pragma once

#include "Module.hpp"

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

	static std::vector<const char*> getInstanceExtensions();

protected:
	Window() = default;

	vk::raii::SurfaceKHR surface{ nullptr };
	vk::SurfaceCapabilitiesKHR surfaceCapabilities;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::PresentModeKHR surfacePresentMode;
	vk::raii::SwapchainKHR swapchain{nullptr};
	std::vector<vk::Image> swapchainImages;
	std::vector<vk::raii::ImageView> swapchainImageViews;

	void initialSurfaceData(const vk::raii::PhysicalDevice& physicalDevice);
	void createSwapchain();
	void createSwapchainImageViews();
};