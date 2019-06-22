#pragma once

#include "defines.h"
#include "initialiser.h"
#include "instance.h"
#include "physicaldevice.h"

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

	VkQueue getComputeQueue() {
		return m_computeQueue;
	}

	VkQueue getPresentQueue() {
		return m_presentQueue;
	}

	VkQueue getGraphicsQueue() {
		return m_graphicsQueue;
	}

	VkCommandPool getCommandPool() {
		return m_commandPool;
	}

	void getComputeCommand(VkCommandBuffer * buffers, uint32_t count) {
		VkCommandBufferAllocateInfo createInfo = initialiser::createCommandBufferAllocateInfo(m_commandPool, count);

		ASSERT(vkAllocateCommandBuffers(m_device, &createInfo, buffers), "UNABLE TO CREATE COMMAND BUFFER");
	}

	void freeComputeCommand(VkCommandBuffer * buffers, uint32_t count) {
		vkFreeCommandBuffers(m_device, m_commandPool, count, buffers);
	}

	~Device();

private:
	Instance * m_instance;
	PhysicalDevice * m_physicalDevice;

	VkQueue m_computeQueue;
	VkQueue m_presentQueue;
	VkQueue m_graphicsQueue;
	VkDevice m_device;
	VkCommandPool m_commandPool;

	void createCommandPool() {
		auto commandPoolInfo = initialiser::createComputePoolInfo(m_physicalDevice->getQueueFamilyIndices().computeIndices);
		ASSERT(vkCreateCommandPool(m_device, &commandPoolInfo, nullptr, &m_commandPool), "Unable to create command pool");
	}
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

	vkGetDeviceQueue(
		m_device,
		physicalDevice->getQueueFamilyIndices().computeIndices,
		0,
		&m_presentQueue
	);

	vkGetDeviceQueue(
		m_device,
		physicalDevice->getQueueFamilyIndices().graphicIndices,
		0,
		&m_graphicsQueue
	);

	createCommandPool();
}

Device::~Device()
{
	vkDestroyCommandPool(m_device, m_commandPool, nullptr);
	vkDestroyDevice(m_device, nullptr);
}