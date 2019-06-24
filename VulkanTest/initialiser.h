#pragma once
#include "Header.h"
#include "defines.h"

namespace initialiser {

	struct PipelineStages {
		VkPipelineShaderStageCreateInfo * pShaderStage;
		VkPipelineVertexInputStateCreateInfo pipelineVertexInput;
		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyInfo;
		VkPipelineViewportStateCreateInfo pipelineViewPortInfo;
		VkPipelineRasterizationStateCreateInfo pipelineRasterizationInfo;
		VkPipelineMultisampleStateCreateInfo pipelineMultisampleInfo;
		VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilInfo;
		VkPipelineColorBlendStateCreateInfo pipelineColorBlendInfo;
		VkPipelineDynamicStateCreateInfo pipelineDynamicInfo;
		VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	};

	VkApplicationInfo createAppInfo() {
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan is hard";
		appInfo.applicationVersion = VK_VERSION_1_0;
		appInfo.engineVersion = VK_VERSION_1_0;
		appInfo.apiVersion = VK_VERSION_1_0;
		return appInfo;
	}

	VkInstanceCreateInfo createInstanceInfo(VkApplicationInfo & appInfo, std::vector<const char *>&extensions, std::vector<const char *>&layers) {
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

	VkDeviceCreateInfo createDeviceInfo(std::vector<VkDeviceQueueCreateInfo> & queueCreateInfos, const VkPhysicalDeviceFeatures &deviceFeatures, std::vector<const char *>&extensions)
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

	VkCommandPoolCreateInfo createComputePoolInfo(const uint32_t& queueFamilyIndex, VkCommandPoolCreateFlags flags = 0) {
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = flags;
		createInfo.queueFamilyIndex = queueFamilyIndex;
		return createInfo;
	}

	VkCommandBufferAllocateInfo createCommandBufferAllocateInfo(VkCommandPool& pool, uint32_t count) {
		VkCommandBufferAllocateInfo createInfo = {};
		createInfo.commandPool = pool;
		createInfo.commandBufferCount = count;
		createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		return createInfo;
	}

	// The FIFO present mode is guaranteed by the spec to be supported
	VkSwapchainCreateInfoKHR createSwapChainInfo(VkSurfaceKHR windowSurface, VkExtent2D swapChainExtent, VkSurfaceCapabilitiesKHR surfaceWindowCapabilities, VkPresentModeKHR swapChainPresentMode = VK_PRESENT_MODE_FIFO_KHR, VkFormat format = VK_FORMAT_B8G8R8A8_UNORM) {
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

		//createInfo.preTransform = preTransform;

		return createInfo;
	}

	VkImageViewCreateInfo createSwapChainImageViewInfo(VkImage image) {
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

	VkShaderModuleCreateInfo createShaderModuleInfo(const std::vector<char>& code) {
		// Do not think of this much
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		return createInfo;
	}

	VkPipelineShaderStageCreateInfo createPipelineShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage = VK_SHADER_STAGE_FRAGMENT_BIT) {
		VkPipelineShaderStageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfo.stage = stage;
		createInfo.module = shaderModule;
		createInfo.pName = "main";
		return createInfo;
	}

	VkPipelineVertexInputStateCreateInfo createPipelineVertexInputStateInfo() {
		VkPipelineVertexInputStateCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		createInfo.vertexBindingDescriptionCount = 0;
		createInfo.pVertexBindingDescriptions = nullptr; // Optional
		createInfo.vertexAttributeDescriptionCount = 0;
		createInfo.pVertexAttributeDescriptions = nullptr; // Optional
		return createInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo createPipelineInputAssemblyInfo() {
		VkPipelineInputAssemblyStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		createInfo.primitiveRestartEnable = VK_FALSE;
		return createInfo;
	}

	VkPipelineViewportStateCreateInfo createPipelineViewportStateInfo(VkViewport& viewport, VkRect2D& scissor) {
		VkPipelineViewportStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		createInfo.viewportCount = 1;
		createInfo.pViewports = &viewport;
		createInfo.scissorCount = 1;
		createInfo.pScissors = &scissor;
		return createInfo;
	}

	VkPipelineRasterizationStateCreateInfo createPipelineRasterizationStateInfo() {
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

	VkPipelineMultisampleStateCreateInfo createPipelineMultisampleStateInfo() {
		VkPipelineMultisampleStateCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		createInfo.sampleShadingEnable = VK_FALSE;
		createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.minSampleShading = 1.0f; // Optional
		createInfo.pSampleMask = nullptr; // Optional
		createInfo.alphaToCoverageEnable = VK_FALSE; // Optional
		createInfo.alphaToOneEnable = VK_FALSE; // Optional
		return createInfo;
	}

	VkPipelineDepthStencilStateCreateInfo createPipelineDepthStencilStateInfo() {
		VkPipelineDepthStencilStateCreateInfo createState = {};
		return createState;
	}

	VkPipelineColorBlendStateCreateInfo createPipelineColorBlendStateInfo(VkPipelineColorBlendAttachmentState& pipelineColorBlendAttachment) {
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

	VkPipelineDynamicStateCreateInfo createPipelineDynamicStateInfo(VkDynamicState * pDynamicStates) {
		VkPipelineDynamicStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		createInfo.dynamicStateCount = sizeof(pDynamicStates) / sizeof(VkDynamicState);
		createInfo.pDynamicStates = pDynamicStates;
		return createInfo;
	}

	VkPipelineLayoutCreateInfo createPipelineLayoutInfo() {
		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.setLayoutCount = 0; // Optional
		createInfo.pSetLayouts = nullptr; // Optional
		createInfo.pushConstantRangeCount = 0; // Optional
		createInfo.pPushConstantRanges = nullptr; // Optional
		return createInfo;
	};

	VkRenderPassCreateInfo createRenderPassInfo(std::vector<VkAttachmentDescription>& attachmentReferences, std::vector<VkSubpassDescription>& subpassDescriptions) {
		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = U(attachmentReferences.size());
		createInfo.pAttachments = attachmentReferences.data();
		createInfo.subpassCount = U(subpassDescriptions.size());
		createInfo.pSubpasses = subpassDescriptions.data();
		return createInfo;
	}

	VkGraphicsPipelineCreateInfo createGrpahicsPipilineInfo(PipelineStages& pipelineStages, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, uint32_t subpassIndex) {
		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = 2; //sizeof(pipelineStages.pShaderStage)/sizeof(VkPipelineShaderStageCreateInfo);
		createInfo.pStages = pipelineStages.pShaderStage;
		createInfo.pVertexInputState = &pipelineStages.pipelineVertexInput;
		createInfo.pInputAssemblyState = &pipelineStages.pipelineInputAssemblyInfo;
		createInfo.pViewportState = &pipelineStages.pipelineViewPortInfo;
		createInfo.pRasterizationState = &pipelineStages.pipelineRasterizationInfo;
		createInfo.pMultisampleState = &pipelineStages.pipelineMultisampleInfo;
		createInfo.pDepthStencilState = nullptr; // &pipelineStages.pipelineDepthStencilInfo; // Optional
		createInfo.pColorBlendState = &pipelineStages.pipelineColorBlendInfo;
		createInfo.pDynamicState = nullptr; //&pipelineStages.pipelineDynamicInfo; // Optional
		createInfo.layout = pipelineLayout;
		createInfo.renderPass = renderPass;

		// the index of the subpass in the render pass where this pipeline will be used
		createInfo.subpass = subpassIndex;
		return createInfo;
	}

	VkFramebufferCreateInfo createFrameBufferInfo(VkImageView * imageViewAttachments, VkRenderPass renderPass, VkExtent2D extent) {
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

	VkCommandBufferBeginInfo createCommandBeginInfo() {
		VkCommandBufferBeginInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		createInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		return createInfo;
	}

	VkRenderPassBeginInfo createRenderPassBeginInfo(VkFramebuffer frameBuffer, VkExtent2D extent, VkRenderPass renderPass, VkClearValue &clearColor) {
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

	VkSemaphoreCreateInfo createSemaphoreInfo() {
		VkSemaphoreCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		return createInfo;
	}

	VkSubmitInfo createSubmitInfo(const VkSemaphore * waitSemaphores, const VkSemaphore * signalSemaphore, VkPipelineStageFlags * waitStages, VkCommandBuffer &commandBuffer) {
		VkSubmitInfo createInfo = {};

		createInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		createInfo.waitSemaphoreCount = sizeof(waitSemaphores)/sizeof(VkSemaphore);
		createInfo.pWaitSemaphores = waitSemaphores;
		createInfo.pWaitDstStageMask = waitStages;
		createInfo.commandBufferCount = 1;
		createInfo.pCommandBuffers = &commandBuffer;
		createInfo.signalSemaphoreCount = sizeof(signalSemaphore) / sizeof(VkSemaphore);
		createInfo.pSignalSemaphores = signalSemaphore;
		return createInfo;
	}
}
