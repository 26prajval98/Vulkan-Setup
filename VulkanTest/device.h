#pragma once

#include "Header.h"

class Device
{
public:
	Device(Instance * instance, PhysicalDevice * physicalDevice);

	VkDevice * pGetDevice() {
		return &m_device;
	}

	VkDevice getDevice() {
		return m_device;
	}

	~Device();

private:
	Instance * m_instance;
	PhysicalDevice * m_physicalDevice;

	VkQueue m_computeQueue;
	VkDevice m_device;
};

Device::Device(Instance * instance, PhysicalDevice * physicalDevice) : m_instance(instance), m_physicalDevice(physicalDevice)
{
	//std::vector<VkDeviceQueueCreateInfo> queueInfos;
	//float priority = 1.0f;
	//queueInfos.push_back(initialiser::createDeviceQueueInfo(physicalDevice->getQueueFamilyIndices().computeIndices, priority));

	//VkDeviceCreateInfo createInfo = initialiser::createDeviceInfo(queueInfos, m_physicalDevice->getDeviceFeatures());

	//VkResult vr = vkCreateDevice(
	//	m_physicalDevice->getPhysicalDevice(),
	//	&createInfo,
	//	nullptr,
	//	&m_device
	//);

	//assert(vr == VK_SUCCESS && "failed to create device");

	//vkGetDeviceQueue(
	//	m_device, 
	//	physicalDevice->getQueueFamilyIndices().computeIndices, 
	//	0, 
	//	&m_computeQueue
	//);

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	float queuePriority = 1.0f;
	for (auto& queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		// Priority influeneces the scheduling of command buffer execution
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Specify device features
	VkPhysicalDeviceFeatures deviceFeatures = {};

	// Creating the logical device i.e combine both the above structures	
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfos[0];
	createInfo.queueCreateInfoCount = static_cast<unsigned int>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	// No need for device specific but done so for compatability with old versions of Vulkan for validation
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	if (enableValidationLayer) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validLayers.size());
		createInfo.ppEnabledLayerNames = validLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	// Create logical device
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

}

Device::~Device()
{
	vkDestroyDevice(m_device, nullptr);
}