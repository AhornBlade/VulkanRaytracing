#include <core/Instance.hpp>
#include <core/Pipeline.hpp>
#include <GLFW_EXT/GLFWWindow.hpp>
#include <pipeline/GraphicsPipeline.hpp>

class VulkanApplication
{
public:
	VulkanApplication()
	{
		instance = Instance(getInstanceExtensions<GLFWWindow>(), getInstanceLayers<GLFWWindow>());
		pipeline = Pipeline{ instance.getPhysicalDevices()[0], 0, 1, getPipelineCreateInfo() };
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
	Pipeline pipeline;
	GLFWWindow window;
	Pipeline pipeline;

	GraphicsPipelineCreateInfo getPipelineCreateInfo() const
	{
		GraphicsPipelineCreateInfo createInfo{};
		createInfo.dynamicStates.push_back(vk::DynamicState::eViewport);
		createInfo.dynamicStates.push_back(vk::DynamicState::eScissor);
		createInfo.inputAssemblyState.setTopology(vk::PrimitiveTopology::eTriangleList);

		auto& rasterizer = createInfo.rasterizationState;
		rasterizer.setCullMode(vk::CullModeFlagBits::eBack);
		rasterizer.setPolygonMode(vk::PolygonMode::eFill);
		rasterizer.setFrontFace(vk::FrontFace::eClockwise);
		rasterizer.setLineWidth(1.0f);

		createInfo.multisampleState.setRasterizationSamples(vk::SampleCountFlagBits::e1);
		createInfo.multisampleState.setMinSampleShading(1.0f);

		vk::PipelineColorBlendAttachmentState colorBlend{};
		colorBlend.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
		colorBlend.setSrcColorBlendFactor(vk::BlendFactor::eOne);
		colorBlend.setDstColorBlendFactor(vk::BlendFactor::eZero);
		colorBlend.setColorBlendOp(vk::BlendOp::eAdd);
		colorBlend.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);
		colorBlend.setDstAlphaBlendFactor(vk::BlendFactor::eZero);
		colorBlend.setAlphaBlendOp(vk::BlendOp::eAdd);
		createInfo.blendAttachmentStates.push_back(colorBlend);
		
		createInfo.colorBlendState.setLogicOp(vk::LogicOp::eCopy);
	}
};

int main()
{
	VulkanApplication app{};
	app.run();
}