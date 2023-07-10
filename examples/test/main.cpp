#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan_raii.hpp>

#ifdef VK_USE_PLATFORM_WIN32_KHR
#define VK_SURFACE_CREATE_INFO vk::Win32SurfaceCreateInfoKHR
#elif(defined(VK_USE_PLATFORM_XCB_KHR))

#endif
#include <vector>
#include <iostream>

class Window
{
public:
	Window() = default;
	Window(int width, int height, const char* title)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	}
	~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) noexcept = delete;
	Window& operator=(Window&&) noexcept = delete;

	std::vector<const char*> getRequiredInstanceExtensions() const noexcept
	{
		uint32_t extensionCount;
		const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
		return { extensions, extensions + extensionCount };
	}

	VK_SURFACE_CREATE_INFO getVulkanSurfaceCreateInfo() const noexcept
	{
#ifdef VK_USE_PLATFORM_WIN32_KHR
		vk::Win32SurfaceCreateInfoKHR createInfo{};
		createInfo.setHinstance(GetModuleHandle(nullptr));
		createInfo.setHwnd(glfwGetWin32Window(window));
		return createInfo;
#endif
	}

private:
	GLFWwindow* window;
};

class VulkanRenderer
{
public:
	VulkanRenderer(const Window& _window) : window{ &_window }
	{
		createInstance();
		choosePhysicalDevice();
	}


private:
	const Window* window;
	vk::raii::Context context;
	vk::raii::Instance instance{nullptr};
	vk::raii::DebugUtilsMessengerEXT debugMessenger{nullptr};
	vk::raii::PhysicalDevice physicalDevice{nullptr};

	void createInstance()
	{
		auto enabledExtensions = window->getRequiredInstanceExtensions();
		std::vector<const char*> enabledLayers{};

#ifdef VULKAN_RAY_TRACING_DEBUG
		enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		enabledLayers.push_back("VK_LAYER_KHRONOS_validation");

		vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};
		debugInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning);
		debugInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
		debugInfo.setPfnUserCallback([](
			VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)->VkBool32
			{
				if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
				{
					std::cout << "Info: " << pCallbackData->pMessage << '\n';
				}
				else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
				{
					std::cout << "Warning: " << pCallbackData->pMessage << '\n';
				}
				else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
				{
					std::cout << "Error: " << pCallbackData->pMessage << '\n';
				}

				return VK_FALSE;
			});

#endif

		vk::InstanceCreateInfo createInfo{};
		createInfo.setPEnabledExtensionNames(enabledExtensions);
		createInfo.setPEnabledLayerNames(enabledLayers);
#ifdef VULKAN_RAY_TRACING_DEBUG
		createInfo.setPNext(&debugInfo);
#endif

		instance = vk::raii::Instance{ context, createInfo };
		debugMessenger = vk::raii::DebugUtilsMessengerEXT{ instance, debugInfo };

		std::cout << "Info: Success to create instance\n";
	}

	void choosePhysicalDevice()
	{
		auto enabledPhysicalDevices = instance.enumeratePhysicalDevices();
		if (enabledPhysicalDevices.empty())
		{
			std::cout << "Error: Failed to find physical devices\n";
		}

		for (auto enabledPhysicalDevice : enabledPhysicalDevices)
		{
			auto properties = enabledPhysicalDevice.getProperties();
			if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
			{
				physicalDevice = enabledPhysicalDevice;
				break;
			}
		}
		if (!(*physicalDevice))
		{
			physicalDevice = enabledPhysicalDevices[0];
		}

		std::cout << "Info: Using physical device " << physicalDevice.getProperties().deviceName << '\n';
	}
};

int main()
{
	Window window{ 800, 600, "test.cpp" };
	VulkanRenderer app{ window };
}