#pragma once

#include "Header.h"
#include "device.h"
#include "initialiser.h"

class Semaphore
{
public:
	Semaphore(Device * device);
	~Semaphore();

	VkSemaphore getImageAvailableSemaphore() {
		return m_imageAvailableSemaphore;
	}

	VkSemaphore getRenderImageFinishedSemaphore() {
		return m_renderImageFinishedSemaphore;
	}

private:
	VkSemaphore m_imageAvailableSemaphore;
	VkSemaphore m_renderImageFinishedSemaphore;

	Device * m_device;
};

Semaphore::Semaphore(Device * device) : m_device(device)
{
	auto createInfo_0 = initialiser::createSemaphoreInfo();
	ASSERT(vkCreateSemaphore(m_device->getDevice(), &createInfo_0, nullptr, &m_imageAvailableSemaphore), "Uable to create Semaphore");
	auto createInfo_1 = initialiser::createSemaphoreInfo();
	ASSERT(vkCreateSemaphore(m_device->getDevice(), &createInfo_1, nullptr, &m_renderImageFinishedSemaphore), "Uable to create Semaphore");
}

Semaphore::~Semaphore()
{
	vkDestroySemaphore(m_device->getDevice(), m_imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(m_device->getDevice(), m_renderImageFinishedSemaphore, nullptr);
}