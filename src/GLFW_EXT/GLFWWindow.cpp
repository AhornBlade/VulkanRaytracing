#include "GLFWWindow.hpp"

GLFWWindow::GLFWWindow(const Instance& instance, const vk::raii::PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	Window(instance, physicalDevice, queueFamilyIndex, getVulkanSurfaceCreateInfo());
}

GLFWWindow::~GLFWWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

std::vector<const char*> GLFWWindow::getInstanceExtensions()
{
	auto extensions = Window::getInstanceExtensions();
#ifdef VK_USE_PLATFORM_WIN32_KHR
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
	return extensions;
}

VK_SURFACE_CREATE_INFO GLFWWindow::getVulkanSurfaceCreateInfo() const noexcept
{
#ifdef VK_USE_PLATFORM_WIN32_KHR
	vk::Win32SurfaceCreateInfoKHR createInfo{};
	createInfo.setHinstance(GetModuleHandle(nullptr));
	createInfo.setHwnd(glfwGetWin32Window(window));
	return createInfo;
#endif
}