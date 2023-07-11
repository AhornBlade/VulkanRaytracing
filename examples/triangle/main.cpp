#include <core/Instance.hpp>
#include <GLFW_EXT/GLFWWindow.hpp>

class VulkanApplication
{
public:
	VulkanApplication()
	{
		instance = Instance(getInstanceExtensions<GLFWWindow>(), getInstanceLayers<GLFWWindow>());
		window = GLFWWindowManager::createGLFWWindow(instance, 
			instance.getPhysicalDevices()[0], 0, 800, 600, "triangle");
	}

	void run()
	{
		while (!window.shouldClose())
		{
			GLFWWindowManager::show();
		}
	}

private:
	Instance instance;
	GLFWWindow window;
};

int main()
{
	VulkanApplication app{};
	app.run();
}