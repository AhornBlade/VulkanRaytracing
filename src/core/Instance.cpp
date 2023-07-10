#include "Instance.hpp"

#include <iostream>

Instance::Instance(const std::vector<const char*>& enabledExtensions, const std::vector<const char*>& enabledLayers)
{
	createInsance(enabledExtensions, enabledLayers);
}

void Instance::createInsance(const std::vector<const char*>& extensions, const std::vector<const char*>& layers)
{
	vk::InstanceCreateInfo createInfo{};

#ifdef VULKAN_RAY_TRACING_DEBUG
	std::vector<const char*> enabledExtensions = extensions;
	std::vector<const char*> enabledLayers = layers;

	enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	enabledLayers.push_back("VK_LAYER_KHRONOS_validation");

	vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};
	debugInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo);
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
	createInfo.setPEnabledExtensionNames(enabledExtensions);
	createInfo.setPEnabledLayerNames(enabledLayers);
	createInfo.setPNext(&debugInfo);
#else
	createInfo.setPEnabledExtensionNames(extensions);
	createInfo.setPEnabledLayerNames(layers);
#endif

	instance = vk::raii::Instance{ context, createInfo };
#ifdef VULKAN_RAY_TRACING_DEBUG
	debugMessenger = vk::raii::DebugUtilsMessengerEXT{ instance, debugInfo };
#endif

	std::cout << "Info: Success to create instance\n";

	physicalDevices = vk::raii::PhysicalDevices(instance);

	std::cout << "Info: Found " << physicalDevices.size() << " physical devices\n";
}
