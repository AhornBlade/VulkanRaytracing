#include <core/Instance.hpp>
#include <core/Pipeline.hpp>
#include <GLFW_EXT/GLFWWindow.hpp>
#include <pipeline/GraphicsPipeline.hpp>

#include <triangle_vert.hpp>
#include <triangle_frag.hpp>

class VulkanApplication
{
public:
	VulkanApplication()
	{
		instance = Instance(getInstanceExtensions<GLFWWindow, Pipeline>(), 
			getInstanceLayers<GLFWWindow, Pipeline>());
		window = GLFWWindowManager::createGLFWWindow(instance, 
			instance.getPhysicalDevices()[0], 0, 800, 600, "triangle");
		pipeline = Pipeline{ instance.getPhysicalDevices()[0], 0 , 1, getPipelineCreateInfo() };

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
	Pipeline pipeline;

	GraphicsPipelineCreateInfo getPipelineCreateInfo()const noexcept
	{
		GraphicsPipelineCreateInfo createInfo{};
		createInfo.extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

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
		
		createInfo.colorBlendLogicOp = vk::LogicOp::eCopy;

		createInfo.shaderCreateInfos.emplace_back(
			ShaderCreateInfo{ Shader::triangle_vert, vk::ShaderStageFlagBits::eVertex });
		createInfo.shaderCreateInfos.emplace_back(
			ShaderCreateInfo{ Shader::triangle_frag, vk::ShaderStageFlagBits::eFragment });

		vk::Extent2D imageExtent = window.getImageExtent();

		vk::Viewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = imageExtent.width;
		viewport.height = imageExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		createInfo.viewports.push_back(viewport);

		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D{ 0, 0 };
		scissor.extent = imageExtent;

		createInfo.scissors.push_back(scissor);

		vk::AttachmentDescription colorAttachment;
		colorAttachment.setFormat(window.getImageFormat());
		colorAttachment.setSamples(vk::SampleCountFlagBits::e1);
		colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
		colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
		colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
		colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
		colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
		colorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		createInfo.attachments.push_back(colorAttachment);

		vk::AttachmentReference colorRef;
		colorRef.setAttachment(0);
		colorRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		SubpassDescription subpass;
		subpass.colorAttachments.push_back(colorRef);
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

		createInfo.subpassDescriptions.push_back(subpass);

		return createInfo;
	}
};

int main()
{
	VulkanApplication app{};
	app.run();
}