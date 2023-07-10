#include <core/Instance.hpp>
#include <GLFW_EXT/GLFWWindow.hpp>

class VulkanApplication
{
public:
	VulkanApplication()
	{
		
	}


private:
	Instance instance;
	GLFWWindow window;
};

int main()
{
	VulkanApplication app{};
}