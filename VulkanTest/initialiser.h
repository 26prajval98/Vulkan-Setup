#pragma once
#include "Header.h"

namespace initialiser {

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
}