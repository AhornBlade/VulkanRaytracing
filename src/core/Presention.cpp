#include "Presention.hpp"

#include <iostream>

Window::Window(const vk::raii::PhysicalDevice& physicalDevice, uint32_t queueFamilyIndex, vk::raii::SurfaceKHR&& _surface) : Module()
{
	surface = std::move(_surface);

	std::vector<const char*> enabledExtension{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	createDevice(physicalDevice, queueFamilyIndex, 1, enabledExtension);
	initialSurfaceData(physicalDevice);
	createSwapchain();
	createSwapchainImageViews();
}

std::vector<const char*> Window::getInstanceExtensions()
{
	return { VK_KHR_SURFACE_EXTENSION_NAME };
}

void Window::initialSurfaceData(const vk::raii::PhysicalDevice& physicalDevice)
{
	auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(*surface);
	surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
	auto surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(*surface);
	surfaceFormat = surfaceFormats[0];
	for (auto& surfaceFormat : surfaceFormats)
	{
		if (surfaceFormat.format == vk::Format::eB8G8R8A8Srgb && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			surfaceFormat = surfaceFormat;
			break;
		}
	}

	surfacePresentMode = surfacePresentModes[0];

	for (auto& presentMode : surfacePresentModes)
	{
		if (presentMode == vk::PresentModeKHR::eMailbox)
		{
			surfacePresentMode = presentMode;
			break;
		}
	}
}

void Window::createSwapchain()
{
	vk::SwapchainCreateInfoKHR createInfo{};
	createInfo.setSurface(*surface);
	createInfo.setOldSwapchain(*swapchain);

	createInfo.setMinImageCount((surfaceCapabilities.minImageCount == surfaceCapabilities.maxImageCount) ?
		surfaceCapabilities.maxImageCount : (surfaceCapabilities.minImageCount + 1));
	createInfo.setImageExtent(surfaceCapabilities.currentExtent);
	createInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
	createInfo.setImageArrayLayers(1);
	createInfo.setImageSharingMode(vk::SharingMode::eExclusive);
	createInfo.setImageFormat(surfaceFormat.format);
	createInfo.setImageColorSpace(surfaceFormat.colorSpace);

	createInfo.setPreTransform(surfaceCapabilities.currentTransform);
	createInfo.setClipped(VK_TRUE);
	createInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
	createInfo.setPresentMode(surfacePresentMode);

	swapchain = vk::raii::SwapchainKHR{ device, createInfo };

	std::cout << "Info: Success to create swapchain\n";
}

void Window::createSwapchainImageViews()
{
	swapchainImages = swapchain.getImages();

	vk::ImageViewCreateInfo imageViewCreateInfo{};
	imageViewCreateInfo.setViewType(vk::ImageViewType::e2D);
	imageViewCreateInfo.setFormat(surfaceFormat.format);
	imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	for (auto& swapchainImage : swapchainImages)
	{
		imageViewCreateInfo.setImage(swapchainImage);
		swapchainImageViews.emplace_back(vk::raii::ImageView{device, imageViewCreateInfo});
	}

	std::cout << "Info: Success to create image views\n";
}