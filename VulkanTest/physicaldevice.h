#pragma once

#include "Header.h"
#include "physicaldevice.h"
#include "windowsurface.h"
#include "queuefamily.h"

class PhysicalDevice
{
public:
	QueueFamilyIndices m_queueFamilyIndices;

	static PhysicalDevice * GetPhysicalDevice(Instance * instance)
	{
		std::vector<VkPhysicalDevice> physicalDevices = getAllPhysicalDevices(instance);

		for (auto& physicalDevice : physicalDevices) {
			QueueFamilyIndices queueFamilyIndices;
			if (isSupported(physicalDevice, instance, queueFamilyIndices)) {
				return new PhysicalDevice(physicalDevice, instance, queueFamilyIndices);
			}
		}

		std::cerr << "No Physical Device Found" << std::endl;
		exit(-1);
	}

	VkPhysicalDevice getPhysicalDevice()
	{
		return m_physicalDevice;
	}

	QueueFamilyIndices getQueueFamilyIndices()
	{
		return m_queueFamilyIndices;
	}

	Instance * getInstance()
	{
		return m_instance;
	}

	VkPhysicalDeviceFeatures getDeviceFeatures() {
		return m_physicalDeviceFeatures;
	}

	VkBool32 doesSupportWindowSurface(WindowSurface * windowSurface) {
		m_windowSurface = windowSurface;
		VkBool32 windowSurfaceSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, m_queueFamilyIndices.computeIndices, m_windowSurface->getWindowSurface(), &windowSurfaceSupport);
		return windowSurfaceSupport;
	}

	~PhysicalDevice();

private:
	Instance * m_instance;
	WindowSurface * m_windowSurface;

	VkPhysicalDevice m_physicalDevice;
	VkPhysicalDeviceProperties m_physicalDeviceProperties;
	VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
	VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;

	PhysicalDevice(VkPhysicalDevice& physicalDevice, Instance * instance, QueueFamilyIndices queueFamilyIndices, WindowSurface * windowSurface = nullptr) : m_instance(instance), m_queueFamilyIndices(queueFamilyIndices), m_physicalDevice(physicalDevice), m_windowSurface(windowSurface)
	{
		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_physicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);
	}

	static std::vector<VkPhysicalDevice> getAllPhysicalDevices(Instance * instance)
	{
		unsigned int physicalDeviceCount = 0;

		vkEnumeratePhysicalDevices(
			instance->getInstance(),
			&physicalDeviceCount,
			nullptr
		);

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);

		vkEnumeratePhysicalDevices(
			instance->getInstance(),
			&physicalDeviceCount,
			physicalDevices.data()
		);

		return physicalDevices;
	}

	static bool isSupported(VkPhysicalDevice& physicalDevice, Instance * instance, QueueFamilyIndices& queueFamilyIndices) {
		uint32_t queueFamilyTypes = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(
			physicalDevice,
			&queueFamilyTypes,
			nullptr
		);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyTypes);

		vkGetPhysicalDeviceQueueFamilyProperties(
			physicalDevice,
			&queueFamilyTypes,
			queueFamilies.data()
		);

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
};


PhysicalDevice::~PhysicalDevice()
{

}
