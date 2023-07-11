#include "Swapchain.hpp"

#include <iostream>

Swapchain::Swapchain(Swapchain&& other) noexcept
	:vk::raii::SwapchainKHR{ std::move(other) },
	images{ std::move(other.images) },
	imageViews{ std::move(other.imageViews) }{}

Swapchain::Swapchain(const vk::raii::Device& device, vk::raii::SwapchainKHR&& swapchain, vk::Format imageFormat)
	: vk::raii::SwapchainKHR{ std::move(swapchain) }
{
	createImageViews(device, imageFormat);
}

Swapchain::Swapchain(const vk::raii::Device& device, const vk::SwapchainCreateInfoKHR& createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	: vk::raii::SwapchainKHR{ device, createInfo, allocator }
{
	createImageViews(device, createInfo.imageFormat);
}

Swapchain& Swapchain::operator=(Swapchain&& other) noexcept
{
	vk::raii::SwapchainKHR::operator=(std::move(other));
	std::swap(images, other.images);
	std::swap(imageViews, other.imageViews);

	return *this;
}

void Swapchain::createImageViews(const vk::raii::Device& device, vk::Format imageFormat)
{
	images = this->getImages();

	vk::ImageViewCreateInfo imageViewCreateInfo{};
	imageViewCreateInfo.setViewType(vk::ImageViewType::e2D);
	imageViewCreateInfo.setFormat(imageFormat);
	imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	for (auto& swapchainImage : images)
	{
		imageViewCreateInfo.setImage(swapchainImage);
		imageViews.emplace_back(vk::raii::ImageView{device, imageViewCreateInfo});
	}

	std::cout << "Info: Success to create image views\n";
}

