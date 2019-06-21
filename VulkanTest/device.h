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
	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	float priority = 1.0f;
	queueInfos.push_back(initialiser::createDeviceQueueInfo(physicalDevice->getQueueFamilyIndices().computeIndices, priority));

	VkDeviceCreateInfo createInfo = initialiser::createDeviceInfo(queueInfos, m_physicalDevice->getDeviceFeatures());

	VkResult vr = vkCreateDevice(
		m_physicalDevice->getPhysicalDevice(),
		&createInfo,
		nullptr,
		&m_device
	);

	assert(vr == VK_SUCCESS && "failed to create device");

	vkGetDeviceQueue(
		m_device, 
		physicalDevice->getQueueFamilyIndices().computeIndices, 
		0, 
		&m_computeQueue
	);
}

Device::~Device()
{
	vkDestroyDevice(m_device, nullptr);
}