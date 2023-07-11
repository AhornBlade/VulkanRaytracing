#include "GLFWWindow.hpp"

std::unique_ptr<GLFWWindowManager> GLFWWindowManager::manager = nullptr;
GLFWWindow GLFWWindowManager::createGLFWWindow(
	const Instance& instance,
	const vk::raii::PhysicalDevice& physicalDevice,
	uint32_t queueFamilyIndex,
	int width, int height, const char* title)
{
	if (manager == nullptr)
		manager = std::make_unique<GLFWWindowManager>();

	return GLFWWindow{ instance, physicalDevice, queueFamilyIndex, width, height, title };
}

GLFWWindow* GLFWWindowManager::getWindowPointer(GLFWwindow* window)
{
	return manager->windowMap[window];
}

void GLFWWindowManager::setWindowMap(GLFWwindow* glfwWindow, GLFWWindow* window)
{
	manager->windowMap[glfwWindow] = window;
}

void GLFWWindowManager::deleteWindowMap(GLFWwindow* glfwWindow)
{
	manager->windowMap.erase(glfwWindow);
}

GLFWWindow::GLFWWindow(const Instance& instance, const vk::raii::PhysicalDevice& physicalDevice, uint32_t queueFamilyIndex, int width, int height, const char* title)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	Window::operator=(Window{ instance, physicalDevice, queueFamilyIndex, getVulkanSurfaceCreateInfo() });
	setWindowCallback();
	GLFWWindowManager::setWindowMap(window, this);
}

GLFWWindow::GLFWWindow(GLFWWindow&& other) noexcept
	: Window{ std::move(other) }, window{ std::exchange(other.window, window) } 
{
	if (window)
		GLFWWindowManager::setWindowMap(window, this);
	if (other.window)
		GLFWWindowManager::setWindowMap(other.window, &other);
}


GLFWWindow& GLFWWindow::operator=(GLFWWindow&& other) noexcept
{
	if (this == &other)
		return *this;
	Window::operator=(std::move(other));
	std::swap(window, other.window);
	if (window)
		GLFWWindowManager::setWindowMap(window, this);
	if (other.window)
		GLFWWindowManager::setWindowMap(other.window, &other);
	return *this;
}

GLFWWindow::~GLFWWindow() noexcept
{
	if (window)
	{
		GLFWWindowManager::deleteWindowMap(window);
		glfwDestroyWindow(window);
	}
}

std::vector<const char*>& GLFWWindow::getInstanceExtensions(std::vector<const char*>& extensions) noexcept
{
	Window::getInstanceExtensions(extensions);
#ifdef VK_USE_PLATFORM_WIN32_KHR
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
	return extensions;
}

bool GLFWWindow::shouldClose() const
{
	return glfwWindowShouldClose(window);
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

void GLFWWindow::setWindowCallback()
{
	glfwSetWindowSizeCallback(window,
		[](GLFWwindow* glfwWindow, int width, int height)
		{
			auto window = GLFWWindowManager::getWindowPointer(glfwWindow);
			auto currentExtent = window->getImageExtent();
			if ((width == currentExtent.width) && (height == currentExtent.height))
				return;
			window->rebuildSwapchain();
		});
}

GLFWWindowManager::GLFWWindowManager()
{
	glfwInit();
}

GLFWWindowManager::~GLFWWindowManager() noexcept
{
	glfwTerminate();
}

void GLFWWindowManager::show()
{
	glfwPollEvents();
}
