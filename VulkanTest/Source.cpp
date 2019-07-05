#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <optional>
#include <set>
#include <assert.h>
#include<algorithm> 

#include "read_file.h"

#define DEBUG
//#define width 800
//#define height 600

namespace initialiser {
#define U( f ) static_cast< uint32_t >( f );
	VkApplicationInfo createAppInfo()
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan is hard";
		appInfo.applicationVersion = VK_VERSION_1_0;
		appInfo.engineVersion = VK_VERSION_1_0;
		appInfo.apiVersion = VK_VERSION_1_0;
		return appInfo;
	}

	VkInstanceCreateInfo createInstanceInfo(VkApplicationInfo& appInfo, std::vector< const char* >& extensions,
		std::vector< const char* >& layers)
	{
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledLayerNames = layers.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());

		return createInfo;
	};

	VkDeviceQueueCreateInfo createDeviceQueueInfo(uint32_t queueFamilyIndex, float& priority)
	{
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueCount = 1;
		queueInfo.queueFamilyIndex = queueFamilyIndex;
		queueInfo.pQueuePriorities = &priority;
		return queueInfo;
	}

	VkDeviceCreateInfo createDeviceInfo(std::vector< VkDeviceQueueCreateInfo >& queueCreateInfos,
		const VkPhysicalDeviceFeatures& deviceFeatures,
		std::vector< const char* >& extensions)
	{
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		return createInfo;
	}

	VkCommandPoolCreateInfo createComputePoolInfo(const uint32_t& queueFamilyIndex, VkCommandPoolCreateFlags flags = 0)
	{
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = flags;
		createInfo.queueFamilyIndex = queueFamilyIndex;
		return createInfo;
	}

	VkRenderPassCreateInfo createRenderPassInfo(std::vector< VkAttachmentDescription >& attachmentReferences,
		std::vector< VkSubpassDescription >& subpassDescriptions)
	{
		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = U(attachmentReferences.size());
		createInfo.pAttachments = attachmentReferences.data();
		createInfo.subpassCount = U(subpassDescriptions.size());
		createInfo.pSubpasses = subpassDescriptions.data();
		return createInfo;
	}

	VkShaderModuleCreateInfo createShaderModuleInfo(const std::vector< char >& code)
	{
		// Do not think of this much
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		return createInfo;
	}

	VkPipelineShaderStageCreateInfo
		createPipelineShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage = VK_SHADER_STAGE_FRAGMENT_BIT)
	{
		VkPipelineShaderStageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfo.stage = stage;
		createInfo.module = shaderModule;
		createInfo.pName = "main";
		return createInfo;
	}

	VkPipelineVertexInputStateCreateInfo createPipelineVertexInputStateInfo()
	{
		VkPipelineVertexInputStateCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		createInfo.vertexBindingDescriptionCount = 0;
		createInfo.pVertexBindingDescriptions = nullptr; //&bindingDescription; // Optional
		createInfo.vertexAttributeDescriptionCount = 0;       // U( attributeDescription.size() );
		createInfo.pVertexAttributeDescriptions = nullptr; // attributeDescription.data(); // Optional
		return createInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo createPipelineInputAssemblyInfo()
	{
		VkPipelineInputAssemblyStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		createInfo.primitiveRestartEnable = VK_FALSE;
		return createInfo;
	}

	VkPipelineViewportStateCreateInfo createPipelineViewportStateInfo(VkViewport& viewport, VkRect2D& scissor)
	{
		VkPipelineViewportStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		createInfo.viewportCount = 1;
		createInfo.pViewports = &viewport;
		createInfo.scissorCount = 1;
		createInfo.pScissors = &scissor;
		return createInfo;
	}

	VkPipelineRasterizationStateCreateInfo createPipelineRasterizationStateInfo()
	{
		VkPipelineRasterizationStateCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		createInfo.depthClampEnable = VK_FALSE;
		createInfo.rasterizerDiscardEnable = VK_FALSE;
		createInfo.polygonMode = VK_POLYGON_MODE_FILL;
		createInfo.lineWidth = 1.0f;
		createInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		createInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		createInfo.depthBiasEnable = VK_FALSE;
		createInfo.depthBiasConstantFactor = 0.0f; // Optional
		createInfo.depthBiasClamp = 0.0f; // Optional
		createInfo.depthBiasSlopeFactor = 0.0f; // Optional
		return createInfo;
	}

	VkPipelineMultisampleStateCreateInfo createPipelineMultisampleStateInfo()
	{
		VkPipelineMultisampleStateCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		createInfo.sampleShadingEnable = VK_FALSE;
		createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.minSampleShading = 1.0f;     // Optional
		createInfo.pSampleMask = nullptr;  // Optional
		createInfo.alphaToCoverageEnable = VK_FALSE; // Optional
		createInfo.alphaToOneEnable = VK_FALSE; // Optional
		return createInfo;
	}

	VkPipelineDepthStencilStateCreateInfo createPipelineDepthStencilStateInfo()
	{
		VkPipelineDepthStencilStateCreateInfo createState = {};
		return createState;
	}

	VkPipelineColorBlendStateCreateInfo
		createPipelineColorBlendStateInfo(VkPipelineColorBlendAttachmentState& pipelineColorBlendAttachment)
	{
		VkPipelineColorBlendStateCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		createInfo.logicOpEnable = VK_FALSE;
		createInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &pipelineColorBlendAttachment;
		// RGBA
		createInfo.blendConstants[0] = 0.0f; // Optional
		createInfo.blendConstants[1] = 0.0f; // Optional
		createInfo.blendConstants[2] = 0.0f; // Optional
		createInfo.blendConstants[3] = 0.0f; // Optional
		return createInfo;
	}

	VkPipelineDynamicStateCreateInfo createPipelineDynamicStateInfo(VkDynamicState* pDynamicStates)
	{
		VkPipelineDynamicStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		createInfo.dynamicStateCount = sizeof(pDynamicStates) / sizeof(VkDynamicState);
		createInfo.pDynamicStates = pDynamicStates;
		return createInfo;
	}

	VkPipelineLayoutCreateInfo createPipelineLayoutInfo(VkDescriptorSetLayout& descriptorSetLayout)
	{
		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.setLayoutCount = 1; // sizeof(descriptorSetLayouts) / sizeof(VkDescriptorSetLayout); // Optional
		createInfo.pSetLayouts = &descriptorSetLayout; // Optional
		createInfo.pushConstantRangeCount = 0;                    // Optional
		createInfo.pPushConstantRanges = nullptr;              // Optional
		return createInfo;
	};

	// VkGraphicsPipelineCreateInfo createGrpahicsPipilineInfo( PipelineStages& pipelineStages,
	//                                                          VkPipelineLayout pipelineLayout, VkRenderPass renderPass,
	//                                                          uint32_t subpassIndex )
	// {
	//    VkGraphicsPipelineCreateInfo createInfo = {};
	//    createInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	//    createInfo.stageCount          = 2; //
	//    sizeof(pipelineStages.pShaderStage)/sizeof(VkPipelineShaderStageCreateInfo); createInfo.pStages             =
	//    pipelineStages.pShaderStage; createInfo.pVertexInputState   = &pipelineStages.pipelineVertexInput;
	//    createInfo.pInputAssemblyState = &pipelineStages.pipelineInputAssemblyInfo;
	//    createInfo.pViewportState      = &pipelineStages.pipelineViewPortInfo;
	//    createInfo.pRasterizationState = &pipelineStages.pipelineRasterizationInfo;
	//    createInfo.pMultisampleState   = &pipelineStages.pipelineMultisampleInfo;
	//    createInfo.pDepthStencilState  = nullptr; // &pipelineStages.pipelineDepthStencilInfo; // Optional
	//    createInfo.pColorBlendState    = &pipelineStages.pipelineColorBlendInfo;
	//    createInfo.pDynamicState       = nullptr; //&pipelineStages.pipelineDynamicInfo; // Optional
	//    createInfo.layout              = pipelineLayout;
	//    createInfo.renderPass          = renderPass;

	//    // the index of the subpass in the render pass where this pipeline will be used
	//    createInfo.subpass = subpassIndex;
	//    return createInfo;
	// }

	VkCommandBufferBeginInfo createCommandBeginInfo()
	{
		VkCommandBufferBeginInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		createInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		return createInfo;
	}

	VkRenderPassBeginInfo createRenderPassBeginInfo(VkFramebuffer frameBuffer, VkExtent2D extent, VkRenderPass renderPass,
		VkClearValue& clearColor)
	{
		VkRenderPassBeginInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		createInfo.renderPass = renderPass;
		createInfo.framebuffer = frameBuffer;
		createInfo.renderArea.offset = { 0, 0 };
		createInfo.renderArea.extent = extent;
		createInfo.clearValueCount = 1;
		createInfo.pClearValues = &clearColor;
		return createInfo;
	}

	VkSemaphoreCreateInfo createSemaphoreInfo()
	{
		VkSemaphoreCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		return createInfo;
	}

	VkSwapchainCreateInfoKHR createSwapChainInfo(VkSurfaceKHR windowSurface, VkExtent2D swapChainExtent,
		VkSurfaceCapabilitiesKHR surfaceWindowCapabilities,
		VkPresentModeKHR swapChainPresentMode = VK_PRESENT_MODE_FIFO_KHR,
		VkFormat format = VK_FORMAT_B8G8R8A8_UNORM)
	{
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = windowSurface;
		createInfo.imageFormat = format;
		createInfo.imageExtent.width = swapChainExtent.width;
		createInfo.imageExtent.height = swapChainExtent.height;
		createInfo.minImageCount = surfaceWindowCapabilities.minImageCount;
		createInfo.imageArrayLayers = 1;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = swapChainPresentMode;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		createInfo.clipped = true;
		createInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = NULL;

		// createInfo.preTransform = preTransform;

		return createInfo;
	}

	VkImageViewCreateInfo createSwapChainImageViewInfo(VkImage image)
	{
		// Do not think of this much
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.flags = 0;
		createInfo.image = image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		return createInfo;
	}

	VkCommandBufferAllocateInfo createCommandBufferAllocateInfo(VkCommandPool& pool, uint32_t count)
	{
		VkCommandBufferAllocateInfo createInfo = {};
		createInfo.commandPool = pool;
		createInfo.commandBufferCount = count;
		createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		return createInfo;
	}

	VkFramebufferCreateInfo createFrameBufferInfo(VkImageView* imageViewAttachments, VkRenderPass renderPass,
		VkExtent2D extent)
	{
		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = renderPass;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = imageViewAttachments;
		createInfo.width = extent.width;
		createInfo.height = extent.height;
		createInfo.layers = 1;
		return createInfo;
	}

	VkFenceCreateInfo createFenceInfo() {
		VkFenceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// create fences in signaled state
		createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		return createInfo;
	}
}

struct QueueFamilyIndices
{
	uint32_t graphicIndices = UINT32_MAX;
	uint32_t computeIndices = UINT32_MAX;
	bool isComplete() { return (graphicIndices < UINT32_MAX && computeIndices < UINT32_MAX); }
};
struct VkRenderPassInfo
{
	VkRenderPass renderPass;
	std::vector< VkAttachmentDescription > attachmentDescriptions;
	std::vector< std::vector< VkAttachmentReference > > attachmentReferences;
	std::vector< VkSubpassDescription > subpassDescriptions;
};


struct VkInfo
{
	// Handling instance, surface and physical device
	VkInstance instance;
	VkSurfaceKHR surface;
	void* ptrPlatformWindow;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceFeatures physicalDeviceFeatures;

	// Handling device
	VkDevice device;
	// queue 0 is compute / present (same for commandPool and commandBuffers)
	// queue 1 is graphics (same for commandPool and commandBuffers)
	VkQueue queues[2];
	std::vector< VkCommandBuffer > commandBuffers[2];
	VkCommandPool commandPool[2];
	std::vector< const char* > instanceExtensions;
	std::vector< const char* > layers;
	std::vector< const char* > deviceExtensions;
	QueueFamilyIndices queueFamilyIndices;

	// Handling renderpass
	VkRenderPassInfo renderPassInfo;

	// Handling Pipeline
	// required for viewport
	VkExtent2D extent;
	VkPipeline graphicsPipeline;

	// Handling draw
	// handling 2 frames in flight must be lesser than max number of image buffers supported by swapchain
	VkSemaphore imageAvailableSemaphores[2];
	VkSemaphore renderFinishedSemaphores[2];
	VkFence inFlightFences[2];

	// Frame buffer in device so that It can be used in recording draw commands
	std::vector< VkFramebuffer > frameBuffer;

	VkSwapchainKHR swapChain;
};

struct SwapChainImages
{
	std::vector< VkImage > image;
	std::vector< VkImageView > view;
};

// Surface is window surface
struct VkSurface
{
	VkSurfaceKHR surface;
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	VkSurfaceFormatKHR surfaceFormat;
	std::vector< VkPresentModeKHR > surfacePresentModes;
};

struct VkSwapChain
{
	VkDevice device;
	VkInstance instance;
	VkSwapchainKHR swapChain;
	VkExtent2D swapChainExtent;
	SwapChainImages swapChainImages;
};







GLFWwindow* window;
VkInfo m_info;
VkSwapChain m_swapChain;
VkSurface m_surface;
VkShaderModule m_vertexShader;
VkShaderModule m_pixelShader;













bool getDeviceQueues(VkPhysicalDevice physicalDevice, QueueFamilyIndices& queueFamilyIndices)
{
	uint32_t queueFamilyTypes = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyTypes, nullptr);

	std::vector< VkQueueFamilyProperties > queueFamilies(queueFamilyTypes);

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyTypes, queueFamilies.data());

	uint32_t i = 0;
	for (auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				queueFamilyIndices.graphicIndices = i;
			if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
				queueFamilyIndices.computeIndices = i;
		}
		if (queueFamilyIndices.isComplete())
		{
			return true;
		}
		queueFamilyIndices.graphicIndices = UINT32_MAX;
		queueFamilyIndices.computeIndices = UINT32_MAX;
		i++;
	}
	return false;
}

std::vector< VkPresentModeKHR > getSurfacePresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	uint32_t countWindowSurfacePresentModes = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &countWindowSurfacePresentModes, nullptr);
	std::vector< VkPresentModeKHR > surfacePresentModes(countWindowSurfacePresentModes);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &countWindowSurfacePresentModes,
		surfacePresentModes.data());
	return surfacePresentModes;
}

void PrepareRenderPass(VkSurfaceFormatKHR surfaceFormat)
{
	VkAttachmentDescription colorAttachmentDescription = {};
	colorAttachmentDescription.format = surfaceFormat.format;
	colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// If depth attachment is there that also must be added
	// It would be great for user to do this himself. Done for ARC purposes only
	m_info.renderPassInfo.attachmentDescriptions = {};
	m_info.renderPassInfo.attachmentDescriptions.push_back(colorAttachmentDescription);

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_info.renderPassInfo.attachmentReferences.resize(1);

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	m_info.renderPassInfo.subpassDescriptions = {};
	VkSubpassDescription subpassDescription = {};
	// Vulkan currently supports graphics subpass but might support compute ones in future
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount =
		static_cast<uint32_t>(m_info.renderPassInfo.attachmentReferences[0].size());
	subpassDescription.pColorAttachments = m_info.renderPassInfo.attachmentReferences[0].data();
	m_info.renderPassInfo.subpassDescriptions.push_back(subpassDescription);

	VkRenderPassCreateInfo createInfo_0 = initialiser::createRenderPassInfo(
		m_info.renderPassInfo.attachmentDescriptions, m_info.renderPassInfo.subpassDescriptions);

	createInfo_0.pDependencies = &dependency;
	createInfo_0.dependencyCount = 1;

	assert(vkCreateRenderPass(m_info.device, &createInfo_0, nullptr, &m_info.renderPassInfo.renderPass) ==
		VK_SUCCESS);

	return;
}


void draw() {
	VkPipelineLayout pipelineLayout = {};

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	VkPipelineViewportStateCreateInfo viewportState = {};
	VkViewport viewport = {};

	// PrepareRenderTarget(); //TODO?

	// SHADERS
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = initialiser::createPipelineShaderStageInfo(
		m_vertexShader, VK_SHADER_STAGE_VERTEX_BIT);
	;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = initialiser::createPipelineShaderStageInfo(
		m_pixelShader, VK_SHADER_STAGE_FRAGMENT_BIT);
	;

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
	// PrepareVertexBuffer();//INCREMENT 2
	// PrepareIndexBuffer();//INCREMENT 2
	// PrepareUniformVariables(); //TODO - Increment 3, for rotating triangle
	// PrepareAttributeVariables(); //Not Used.

	// Vertex Input Info
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = initialiser::createPipelineVertexInputStateInfo();

	// Input Assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = initialiser::createPipelineInputAssemblyInfo();

	// VIEWPORT
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_info.extent.width;
	viewport.height = (float)m_info.extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = m_info.extent;

	viewportState = initialiser::createPipelineViewportStateInfo(viewport, scissor);

	auto pipelineRasterizationInfo = initialiser::createPipelineRasterizationStateInfo();

	auto pipelineMultisampleInfo = initialiser::createPipelineMultisampleStateInfo();

	// auto pipelineDepthStencilInfo = initialiser::createPipelineDepthStencilStateInfo();

	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachment = {};
	pipelineColorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	pipelineColorBlendAttachment.blendEnable = VK_FALSE;
	pipelineColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
	pipelineColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	pipelineColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;      // Optional
	pipelineColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
	pipelineColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	pipelineColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;      // Optional
	auto pipelineColorBlendInfo = initialiser::createPipelineColorBlendStateInfo(pipelineColorBlendAttachment);

	// VkDynamicState pDynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};
	// auto pipelineDynamicInfo        = initialiser::createPipelineDynamicStateInfo( pDynamicStates );

	// LAYOUT
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};

	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	if (vkCreatePipelineLayout(m_info.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &pipelineRasterizationInfo;
	pipelineInfo.pMultisampleState = &pipelineMultisampleInfo;
	pipelineInfo.pColorBlendState = &pipelineColorBlendInfo;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = m_info.renderPassInfo.renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.pDepthStencilState = nullptr; // &pipelineStages.pipelineDepthStencilInfo; // Optional
	pipelineInfo.pDynamicState = nullptr; //&pipelineStages.pipelineDynamicInfo; // Optional
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(m_info.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
		&(m_info.graphicsPipeline)) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(m_info.device, m_pixelShader, nullptr);
	vkDestroyShaderModule(m_info.device, m_vertexShader, nullptr);

	// createCommandBuffers( indexCount, 1, startIndexLocation, 0 );

	// uint32_t instanceCount = 1;
	// uint32_t firstInstance = 0;
	// uint32_t offset        = 0;
	// uint32_t vertexCount   = 3;

	uint32_t i = 0;
	for (auto& cb : m_info.commandBuffers[1])
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		if (vkBeginCommandBuffer(cb, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_info.renderPassInfo.renderPass;
		renderPassInfo.framebuffer = m_info.frameBuffer[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_info.extent;

		VkClearValue clearColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(cb, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_info.graphicsPipeline);

		vkCmdDraw(cb, 3, 1, 0, 0);

		vkCmdEndRenderPass(cb);

		if (vkEndCommandBuffer(cb) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
		++i;
	}

	uint32_t currentFrame = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		uint32_t imageIndex;
		vkWaitForFences(m_info.device, 1, &m_info.inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
		vkResetFences(m_info.device, 1, &m_info.inFlightFences[currentFrame]);
		vkAcquireNextImageKHR(m_info.device, m_info.swapChain, std::numeric_limits< uint64_t >::max(),
			m_info.imageAvailableSemaphores[1], VK_NULL_HANDLE, &imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_info.imageAvailableSemaphores[1] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_info.commandBuffers[1][imageIndex];

		VkSemaphore signalSemaphores[] = { m_info.renderFinishedSemaphores[1] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_info.queues[1], 1, &submitInfo, m_info.inFlightFences[currentFrame]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_info.swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(m_info.queues[0], &presentInfo);
		currentFrame = (currentFrame + 1) % 2;
	}

	//vkDeviceWaitIdle(m_info.device);
}

void createSwapChain() {
	//auto& ptrPlatformWindow = static_cast<Android::IAndroidPlatformWindow&>(window);
	//void* ptrWindowSurface = ptrPlatformWindow.GetNativeWindow();
	//// ANativeWindow aNativeWindow;
	//// ANativeWindow_acquire( &aNativeWindow );
	//auto width = window.GetBounds().GetWidth().GetValue();
	//auto height = window.GetBounds().GetHeight().GetValue();

	//VkAndroidSurfaceCreateInfoKHR createInfo_0 = {};
	//createInfo_0.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	//createInfo_0.pNext = nullptr;
	//createInfo_0.flags = 0;
	//createInfo_0.window = (ANativeWindow*)ptrWindowSurface;
	//auto t = vkCreateAndroidSurfaceKHR(m_info.instance, &createInfo_0, nullptr, &m_surface.surface);
	//assert(t == VK_SUCCESS);
	m_surface.surfacePresentModes = getSurfacePresentModes(m_info.physicalDevice, m_surface.surface);

	VkBool32 windowSurfaceSupport = VK_FALSE;
	VkResult t = vkGetPhysicalDeviceSurfaceSupportKHR(m_info.physicalDevice, m_info.queueFamilyIndices.computeIndices, m_surface.surface, &windowSurfaceSupport);

	assert(windowSurfaceSupport == VK_TRUE && "Not supported");

	assert(m_info.queueFamilyIndices.graphicIndices == m_info.queueFamilyIndices.computeIndices);

	assert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_info.physicalDevice, m_surface.surface,
		&m_surface.surfaceCapabilities) == VK_SUCCESS);

	if (m_surface.surfaceCapabilities.currentExtent.width == 0xFFFFFFFF)
	{
		m_swapChain.swapChainExtent.width =
			std::max(std::min(static_cast<uint32_t>(800), m_surface.surfaceCapabilities.maxImageExtent.width),
				m_surface.surfaceCapabilities.minImageExtent.width);
		m_swapChain.swapChainExtent.height =
			std::max(std::min(static_cast<uint32_t>(600), m_surface.surfaceCapabilities.maxImageExtent.height),
				m_surface.surfaceCapabilities.minImageExtent.height);
	}
	else
	{
		m_swapChain.swapChainExtent = m_surface.surfaceCapabilities.currentExtent;
	}

	VkSwapchainCreateInfoKHR createInfo_1 = initialiser::createSwapChainInfo(
		m_surface.surface, m_swapChain.swapChainExtent, m_surface.surfaceCapabilities);

	createInfo_1.preTransform = m_surface.surfaceCapabilities.currentTransform;

	// temp for query Surface Format
	uint32_t temp_qSurfaceFormat = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_info.physicalDevice, m_surface.surface, &temp_qSurfaceFormat,
		nullptr);

	vkGetPhysicalDeviceSurfaceFormatsKHR(m_info.physicalDevice, m_surface.surface, &temp_qSurfaceFormat,
		&m_surface.surfaceFormat);

	createInfo_1.imageFormat = m_surface.surfaceFormat.format;

	vkCreateSwapchainKHR(m_info.device, &createInfo_1, nullptr, &m_swapChain.swapChain);

	uint32_t swapChainImagesCount = 0;
	assert(vkGetSwapchainImagesKHR(m_info.device, m_swapChain.swapChain, &swapChainImagesCount, nullptr) ==
		VK_SUCCESS);

	m_swapChain.swapChainImages.image.resize(swapChainImagesCount);
	m_swapChain.swapChainImages.view.resize(swapChainImagesCount);

	assert(vkGetSwapchainImagesKHR(m_info.device, m_swapChain.swapChain, &swapChainImagesCount,
		m_swapChain.swapChainImages.image.data()) == VK_SUCCESS);

	int i = 0;
	for (auto& image : m_swapChain.swapChainImages.image)
	{
		auto createInfo_2 = initialiser::createSwapChainImageViewInfo(image);

		assert(vkCreateImageView(m_info.device, &createInfo_2, nullptr, &m_swapChain.swapChainImages.view[i]) ==
			VK_SUCCESS);
		++i;
	}
	PrepareRenderPass(m_surface.surfaceFormat);

	m_info.commandBuffers[0].resize(swapChainImagesCount);
	m_info.commandBuffers[1].resize(swapChainImagesCount);
	m_info.frameBuffer.resize(swapChainImagesCount);

	VkCommandBufferAllocateInfo createInfo_3 =
		initialiser::createCommandBufferAllocateInfo(m_info.commandPool[0], i);
	VkCommandBufferAllocateInfo createInfo_4 =
		initialiser::createCommandBufferAllocateInfo(m_info.commandPool[1], i);

	vkAllocateCommandBuffers(m_info.device, &createInfo_3, m_info.commandBuffers[0].data());
	vkAllocateCommandBuffers(m_info.device, &createInfo_4, m_info.commandBuffers[1].data());

	i = 0;

	for (auto& fb : m_info.frameBuffer)
	{
		VkImageView imageViewAttachments[] = { m_swapChain.swapChainImages.view[i] };
		auto createInfo_5 = initialiser::createFrameBufferInfo(
			imageViewAttachments, m_info.renderPassInfo.renderPass, m_swapChain.swapChainExtent);
		assert(vkCreateFramebuffer(m_info.device, &createInfo_5, nullptr, &fb) == VK_SUCCESS);
		++i;
	}
	m_swapChain.device = m_info.device;
	m_swapChain.instance = m_info.instance;
	m_info.extent = m_swapChain.swapChainExtent;
	m_info.swapChain = m_swapChain.swapChain;
}

void device() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	m_info.instanceExtensions = std::vector< const char* >(glfwExtensions, glfwExtensions + glfwExtensionCount);
	m_info.instanceExtensions.push_back("VK_EXT_debug_report");
	m_info.layers = { "VK_LAYER_LUNARG_standard_validation" };
	VkApplicationInfo createInfo_0 = initialiser::createAppInfo();
	VkInstanceCreateInfo createInfo_1 = initialiser::createInstanceInfo(createInfo_0, m_info.instanceExtensions, m_info.layers);
	auto t = vkCreateInstance(&createInfo_1, nullptr, &m_info.instance);
	assert(t == VK_SUCCESS);

	auto sad = glfwCreateWindowSurface(m_info.instance, window, nullptr, &m_surface.surface);

	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(m_info.instance, &physicalDeviceCount, nullptr);

	assert(physicalDeviceCount > 0);

	std::vector< VkPhysicalDevice > physicalDevices(physicalDeviceCount);

	vkEnumeratePhysicalDevices(m_info.instance, &physicalDeviceCount, physicalDevices.data());

	if (physicalDeviceCount == 1)
		m_info.physicalDevice = physicalDevices[0];

	assert(getDeviceQueues(m_info.physicalDevice, m_info.queueFamilyIndices) == true);

	vkGetPhysicalDeviceFeatures(m_info.physicalDevice, &m_info.physicalDeviceFeatures);

	std::vector< VkDeviceQueueCreateInfo > queueInfos;

	m_info.deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	float priority = 1.0f;

	queueInfos.push_back(initialiser::createDeviceQueueInfo(m_info.queueFamilyIndices.computeIndices, priority));

	VkDeviceCreateInfo createInfo_2 =
		initialiser::createDeviceInfo(queueInfos, m_info.physicalDeviceFeatures, m_info.deviceExtensions);

	assert(vkCreateDevice(m_info.physicalDevice, &createInfo_2, nullptr, &m_info.device) == VK_SUCCESS);

	vkGetDeviceQueue(m_info.device, m_info.queueFamilyIndices.computeIndices, 0, &m_info.queues[0]);
	vkGetDeviceQueue(m_info.device, m_info.queueFamilyIndices.graphicIndices, 0, &m_info.queues[1]);

	auto createInfo_3 = initialiser::createComputePoolInfo(m_info.queueFamilyIndices.computeIndices);
	assert(vkCreateCommandPool(m_info.device, &createInfo_3, nullptr, &m_info.commandPool[0]) == VK_SUCCESS);

	auto createInfo_4 = initialiser::createComputePoolInfo(m_info.queueFamilyIndices.graphicIndices);
	assert(vkCreateCommandPool(m_info.device, &createInfo_4, nullptr, &m_info.commandPool[1]) == VK_SUCCESS);

	for (int i = 0; i < 2; i++)
	{
		auto createInfo_5 = initialiser::createSemaphoreInfo();
		assert(vkCreateSemaphore(m_info.device, &createInfo_5, nullptr, &m_info.imageAvailableSemaphores[i]) ==
			VK_SUCCESS);
		auto createInfo_6 = initialiser::createSemaphoreInfo();
		assert(vkCreateSemaphore(m_info.device, &createInfo_6, nullptr, &m_info.renderFinishedSemaphores[i]) == VK_SUCCESS);
		auto createInfo_7 = initialiser::createFenceInfo();
		assert(vkCreateFence(m_info.device, &createInfo_7, nullptr, &m_info.inFlightFences[i]) == VK_SUCCESS);
	}
}



int main() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

	m_info = {};
	device();
	createSwapChain();
	auto code_vertex = readFile("vert.spv");
	auto createInfo_0 = initialiser::createShaderModuleInfo(code_vertex);
	assert(vkCreateShaderModule(m_info.device, &createInfo_0, nullptr, &m_vertexShader) == VK_SUCCESS);

	auto code_fragment = readFile("frag.spv");
	auto createInfo_1 = initialiser::createShaderModuleInfo(code_fragment);
	assert(vkCreateShaderModule(m_info.device, &createInfo_1, nullptr, &m_pixelShader) == VK_SUCCESS);
	draw();
}