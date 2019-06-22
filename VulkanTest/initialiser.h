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

	VkInstanceCreateInfo createInfo(VkApplicationInfo & appInfo, std::vector<const char *>&extensions, std::vector<const char *>&layers) {
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

	VkDeviceCreateInfo createDeviceInfo(std::vector<VkDeviceQueueCreateInfo> & queueCreateInfos, const VkPhysicalDeviceFeatures &deviceFeatures)
	{
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		return createInfo;
	}

	VkCommandPoolCreateInfo createComputePoolInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0) {
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
}