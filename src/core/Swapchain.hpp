#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <vector>

class Swapchain:public vk::raii::SwapchainKHR
{
public:
	Swapchain(std::nullptr_t) : vk::raii::SwapchainKHR{ nullptr }{}
	Swapchain(Swapchain&& other) noexcept;
	Swapchain(const vk::raii::Device& device, vk::raii::SwapchainKHR&& swapchain, vk::Format imageFormat);
	Swapchain(const vk::raii::Device& device, const vk::SwapchainCreateInfoKHR& createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	Swapchain& operator=(Swapchain&& other)noexcept;

	struct ImageRef
	{
		vk::Image& image;
		vk::raii::ImageView& imageView;
	};

	ImageRef getImage(size_t index)
	{
		return { images[index], imageViews[index] };
	}

private:
	std::vector<vk::Image> images;
	std::vector<vk::raii::ImageView> imageViews;

	void createImageViews(const vk::raii::Device& device, vk::Format imageFormat);
};