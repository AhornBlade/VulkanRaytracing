#include "Presention.hpp"

#include <iostream>

Window::Window(const vk::raii::PhysicalDevice& _physicalDevice, uint32_t queueFamilyIndex, vk::raii::SurfaceKHR&& _surface) 
	: Module(_physicalDevice, queueFamilyIndex, 1, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }), 
	physicalDevice{ _physicalDevice }, surface{ std::move(_surface) }
{
	initialSwapchainCreateInfo();
	swapchain = Swapchain{ device, swapchainCreateInfo };
}

Window::Window(Window&& other) noexcept
	: Module{ std::move(other) }, physicalDevice{ std::move(other.physicalDevice) },
	surface{ std::move(other.surface) },
	swapchainCreateInfo{ std::move(other.swapchainCreateInfo) }, 
	swapchain{ std::move(other.swapchain) } {}

Window& Window::operator=(Window&& other) noexcept
{
	if (this == &other)
		return *this;
	Module::operator=(std::move(other));
	std::swap(physicalDevice, other.physicalDevice);
	std::swap(surface, other.surface);
	std::swap(swapchainCreateInfo, other.swapchainCreateInfo);
	std::swap(swapchain, other.swapchain);

	return *this;
}

std::vector<const char*>& Window::getInstanceExtensions(std::vector<const char*>& extensions) noexcept
{
	Module::getInstanceExtensions(extensions);
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	return extensions;
}

void Window::rebuildSwapchain()
{
	updateSwapchainCreateInfo();
	swapchain = Swapchain{ device, swapchainCreateInfo };
}

void Window::initialSwapchainCreateInfo()
{
	auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(*surface);
	auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
	auto surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(*surface);

	vk::SurfaceFormatKHR enabledFormat = surfaceFormats[0];
	for (auto& surfaceFormat : surfaceFormats)
	{
		if (surfaceFormat.format == vk::Format::eB8G8R8A8Srgb && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			enabledFormat = surfaceFormat;
			break;
		}
	}

	vk::PresentModeKHR enabledPresentMode = surfacePresentModes[0];

	for (auto& presentMode : surfacePresentModes)
	{
		if (presentMode == vk::PresentModeKHR::eMailbox)
		{
			enabledPresentMode = presentMode;
			break;
		}
	}

	swapchainCreateInfo.setSurface(*surface);

	swapchainCreateInfo.setMinImageCount((surfaceCapabilities.minImageCount == surfaceCapabilities.maxImageCount) ?
		surfaceCapabilities.maxImageCount : (surfaceCapabilities.minImageCount + 1));
	swapchainCreateInfo.setImageExtent(surfaceCapabilities.currentExtent);
	swapchainCreateInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
	swapchainCreateInfo.setImageArrayLayers(1);
	swapchainCreateInfo.setImageSharingMode(vk::SharingMode::eExclusive);
	swapchainCreateInfo.setImageFormat(enabledFormat.format);
	swapchainCreateInfo.setImageColorSpace(enabledFormat.colorSpace);

	swapchainCreateInfo.setPreTransform(surfaceCapabilities.currentTransform);
	swapchainCreateInfo.setClipped(VK_TRUE);
	swapchainCreateInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
	swapchainCreateInfo.setPresentMode(enabledPresentMode);
}

void Window::updateSwapchainCreateInfo()
{
	auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
	swapchainCreateInfo.setMinImageCount((surfaceCapabilities.minImageCount == surfaceCapabilities.maxImageCount) ?
		surfaceCapabilities.maxImageCount : (surfaceCapabilities.minImageCount + 1));
	swapchainCreateInfo.setImageExtent(surfaceCapabilities.currentExtent);
	swapchainCreateInfo.setPreTransform(surfaceCapabilities.currentTransform);
	swapchainCreateInfo.setOldSwapchain(*swapchain);
}