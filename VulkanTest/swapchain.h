#pragma once

#include "Header.h"
#include "device.h"
#include "initialiser.h"
#include "physicaldevice.h"
#include "windowsurface.h"

struct StructSwapChainImages {
	std::vector<VkImage> image;
	std::vector<VkImageView> view;
};


class SwapChain
{
public:
	SwapChain(Device * device, PhysicalDevice * physicalDevice, WindowSurface * windowSurface);
	~SwapChain();

	VkExtent2D getSwapChainExtent() {
		return m_swapChainExtent;
	}

	VkSurfaceFormatKHR getSurfaceFormat() {
		return m_surfaceFormat;
	}

	
	std::vector<VkImage> getImages() {
		return m_swapChainImages.image;
	}

	std::vector<VkImageView> getImageViews() {
		return m_swapChainImages.view;
	}

private:
	VkSurfaceFormatKHR m_surfaceFormat;
	VkSwapchainKHR m_swapChain;
	VkExtent2D m_swapChainExtent;
	StructSwapChainImages m_swapChainImages;

	Device * m_device;
	PhysicalDevice * m_physicalDevice;
	WindowSurface * m_windowSurface;

	void setSize(uint32_t size) {

		m_swapChainImages.image.resize(size);
		m_swapChainImages.view.resize(size);
		
		checkImageSize();
	}

	void checkImageSize() {
		ASSERT_VALUE(static_cast<uint32_t>(m_swapChainImages.image.size()), static_cast<uint32_t>(m_swapChainImages.view.size()), "IMAGES AND VIEWS ARE DIFFERENT SIZES");
	}
};

SwapChain::SwapChain(Device * device, PhysicalDevice * physicalDevice, WindowSurface * windowSurface) : m_device(device), m_physicalDevice(physicalDevice), m_windowSurface(windowSurface)
{
	auto pd = m_physicalDevice->getPhysicalDevice();
	auto surfaceWindowCapabilities = m_windowSurface->getSurfaceWindowCapabilities(pd);
	auto surfacePresentModes = m_windowSurface->getSurfacePresentModes(pd);

	if (surfaceWindowCapabilities.currentExtent.width == 0xFFFFFFFF) {
		m_swapChainExtent.width = std::max(std::min(static_cast<uint32_t>(WIDTH), surfaceWindowCapabilities.maxImageExtent.width), surfaceWindowCapabilities.minImageExtent.width);
		m_swapChainExtent.height = std::max(std::min(static_cast<uint32_t>(HEIGHT), surfaceWindowCapabilities.maxImageExtent.height), surfaceWindowCapabilities.minImageExtent.height);
	}
	else {
		m_swapChainExtent = surfaceWindowCapabilities.currentExtent;
	}

	//VkSharingMode swapChainSharingMode = VK_SHARING_MODE_CONCURRENT;

	VkSwapchainCreateInfoKHR createInfo = initialiser::createSwapChainInfo(m_windowSurface->getWindowSurface(), m_swapChainExtent, surfaceWindowCapabilities);

	createInfo.preTransform = surfaceWindowCapabilities.currentTransform;

	uint32_t queueFamily[] = { physicalDevice->getQueueFamilyIndices().graphicIndices, physicalDevice->getQueueFamilyIndices().computeIndices };

	if (physicalDevice->getQueueFamilyIndices().graphicIndices == physicalDevice->getQueueFamilyIndices().computeIndices) {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = sizeof(queueFamily) / sizeof(uint32_t);
		createInfo.pQueueFamilyIndices = queueFamily;
	}


	uint32_t t_0 = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->getPhysicalDevice(), m_windowSurface->getWindowSurface(), &t_0, nullptr);

	vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->getPhysicalDevice(), m_windowSurface->getWindowSurface(), &t_0, &m_surfaceFormat);

	createInfo.imageFormat = m_surfaceFormat.format;

	ASSERT(vkCreateSwapchainKHR(m_device->getDevice(), &createInfo, nullptr, &m_swapChain), "UNABLE TO FORM SWAPCHAIN");

	uint32_t swapChainImagesCount = 0;
	ASSERT(vkGetSwapchainImagesKHR(m_device->getDevice(), m_swapChain, &swapChainImagesCount, nullptr), "UNABLE TO FORM IMAGES");

	setSize(swapChainImagesCount);

	ASSERT(vkGetSwapchainImagesKHR(m_device->getDevice(), m_swapChain, &swapChainImagesCount, m_swapChainImages.image.data()), "UNABLE TO FORM IMAGES");
	
	int i = 0;
	for (auto& image : m_swapChainImages.image) {
		auto createInfo = initialiser::createSwapChainImageViewInfo(image);

		ASSERT(vkCreateImageView(m_device->getDevice(), &createInfo, nullptr, &m_swapChainImages.view[i]), "IMAGE VIEWS CANNOT BE CREATED");
		++i;
	}
	checkImageSize();
}

SwapChain::~SwapChain()
{
	//uint32_t swapChainImagesCount = static_cast<uint32_t>(m_swapChainImages.image.size());
	for (auto& view : m_swapChainImages.view) {
		vkDestroyImageView(m_device->getDevice(), view, nullptr);
	}

	vkDestroySwapchainKHR(m_device->getDevice(), m_swapChain, nullptr);
}