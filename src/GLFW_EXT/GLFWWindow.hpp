#pragma once

#include <Presention.hpp>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <map>

#ifdef VK_USE_PLATFORM_WIN32_KHR
#define VK_SURFACE_CREATE_INFO vk::Win32SurfaceCreateInfoKHR
#elif(defined(VK_USE_PLATFORM_XCB_KHR))

#endif

class GLFWWindow;

class GLFWWindowManager
{
public:
	GLFWWindowManager();
	~GLFWWindowManager() noexcept;

	static std::unique_ptr<GLFWWindowManager> manager;
	static GLFWWindow createGLFWWindow(
		const Instance& instance,
		const vk::raii::PhysicalDevice& physicalDevice,
		uint32_t queueFamilyIndex,
		int width, int height, const char* title);

	static void show();

	static GLFWWindow* getWindowPointer(GLFWwindow* window);
	static void setWindowMap(GLFWwindow* glfwWindow, GLFWWindow* window);
	static void deleteWindowMap(GLFWwindow* glfwWindow);

private:
	std::map<GLFWwindow*, GLFWWindow*> windowMap;
};

class GLFWWindow: public Window
{
public:
	GLFWWindow() = default;

	GLFWWindow(GLFWWindow&& other) noexcept;
	GLFWWindow& operator=(GLFWWindow&& other) noexcept;

	virtual ~GLFWWindow() noexcept override;
	static std::vector<const char*>& getInstanceExtensions(std::vector<const char*>& extensions) noexcept;
	bool shouldClose() const;

	friend class GLFWWindowManager;

private:
	GLFWWindow(
		const Instance& instance, 
		const vk::raii::PhysicalDevice& physicalDevice, 
		uint32_t queueFamilyIndex,
		int width, int height, const char* title);

	GLFWwindow* window = nullptr;

	VK_SURFACE_CREATE_INFO getVulkanSurfaceCreateInfo() const noexcept;
	void setWindowCallback();
};
