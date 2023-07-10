#pragma once

#include <Presention.hpp>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#ifdef VK_USE_PLATFORM_WIN32_KHR
#define VK_SURFACE_CREATE_INFO vk::Win32SurfaceCreateInfoKHR
#elif(defined(VK_USE_PLATFORM_XCB_KHR))

#endif

class GLFWWindow: public Window
{
public:
	GLFWWindow(
		const Instance& instance, 
		const vk::raii::PhysicalDevice physicalDevice, 
		uint32_t queueFamilyIndex,
		int width, int height, const char* title);

	virtual ~GLFWWindow() override;

	static std::vector<const char*> getInstanceExtensions();

private:
	GLFWwindow* window = nullptr;

	VK_SURFACE_CREATE_INFO getVulkanSurfaceCreateInfo() const noexcept;

};