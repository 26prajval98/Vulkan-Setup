pragma once

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

	VkCommandPool getCommandPool(bool isGraphicCommandPool = false) {
		if (isGraphicCommandPool)
			return m_commandPool[1];
		return m_commandPool[0];
	}

	void getComputeCommand(VkCommandBuffer * buffers, uint32_t count) {
		VkCommandBufferAllocateInfo createInfo = initialiser::createCommandBufferAllocateInfo(m_commandPool[0], count);
		ASSERT(vkAllocateCommandBuffers(m_device, &createInfo, buffers), "UNABLE TO CREATE COMMAND BUFFER");
	}

	void freeComputeCommand(VkCommandBuffer * buffers, uint32_t count) {
		vkFreeCommandBuffers(m_device, m_commandPool[0], count, buffers);
	}

	void getGraphicsCommand(VkCommandBuffer * buffers, uint32_t count) {
		VkCommandBufferAllocateInfo createInfo = initialiser::createCommandBufferAllocateInfo(m_commandPool[1], count);
		ASSERT(vkAllocateCommandBuffers(m_device, &createInfo, buffers), "UNABLE TO CREATE COMMAND BUFFER");
	}

	void freeGraphicsCommand(VkCommandBuffer * buffers, uint32_t count) {
		vkFreeCommandBuffers(m_device, m_commandPool[1], count, buffers);
	}

	~Device();

private:
	Instance * m_instance;
	PhysicalDevice * m_physicalDevice;

	VkQueue m_computeQueue;
	VkQueue m_presentQueue;
	VkQueue m_graphicsQueue;
	VkDevice m_device;
	VkCommandPool m_commandPool[2];

	void createCommandPool() {
		auto commandPoolInfo_0 = initialiser::createComputePoolInfo(m_physicalDevice->getQueueFamilyIndices().computeIndices);
		ASSERT(vkCreateCommandPool(m_device, &commandPoolInfo_0, nullptr, &m_commandPool[0]), "Unable to create command pool");

		auto commandPoolInfo_1 = initialiser::createComputePoolInfo(m_physicalDevice->getQueueFamilyIndices().graphicIndices);
		ASSERT(vkCreateCommandPool(m_device, &commandPoolInfo_1, nullptr, &m_commandPool[1]), "Unable to create command pool");

	}
};

Device::Device(Instance * instance, PhysicalDevice * physicalDevice) : m_instance(instance), m_physicalDevice(physicalDevice)
{
	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	//VK_KHR_SWAPCHAIN_EXTENSION_NAME is same as  "VK_KHR_swapchain"
	std::vector<const char *> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	float priority = 1.0f;
	queueInfos.push_back(initialiser::createDeviceQueueInfo(physicalDevice->getQueueFamilyIndices().computeIndices, priority));

	VkDeviceCreateInfo createInfo = initialiser::createDeviceInfo(queueInfos, m_physicalDevice->getDeviceFeatures(), extensions);

	ASSERT(vkCreateDevice(
		m_physicalDevice->getPhysicalDevice(),
		&createInfo,
		nullptr,
		&m_device
	), "failed to create device");

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
	vkDestroyCommandPool(m_device, m_commandPool[0], nullptr);
	vkDestroyCommandPool(m_device, m_commandPool[1], nullptr);
	vkDestroyDevice(m_device, nullptr);
}