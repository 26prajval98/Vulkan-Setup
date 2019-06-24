#pragma once

// Added addition of fences for CPU and GPU sync

#include "Header.h"
#include "device.h"
#include "defines.h"
#include "initialiser.h"

const uint32_t MAX_FRAMES_IN_FLIGHT = MM;

class Semaphore
{
public:
	Semaphore(Device * device);
	~Semaphore();

	VkSemaphore getImageAvailableSemaphore(uint32_t i = 0) {
		i %= MAX_FRAMES_IN_FLIGHT;
		return m_imageAvailableSemaphores[i];
	}

	VkSemaphore getRenderImageFinishedSemaphore(uint32_t i = 0) {
		i %= MAX_FRAMES_IN_FLIGHT;
		return m_renderImageFinishedSemaphores[i];
	}

	VkFence getFence(uint32_t i = 0) {
		i %= MAX_FRAMES_IN_FLIGHT;
		return m_fences[i];
	}

	VkSemaphore* pGetImageAvailableSemaphore(uint32_t i = 0) {
		i %= MAX_FRAMES_IN_FLIGHT;
		return &m_imageAvailableSemaphores[i];
	}

	VkSemaphore* pGetRenderImageFinishedSemaphore(uint32_t i = 0) {
		i %= MAX_FRAMES_IN_FLIGHT;
		return &m_renderImageFinishedSemaphores[i];
	}

	VkFence * pGetFence(uint32_t i = 0) {
		i %= MAX_FRAMES_IN_FLIGHT;
		return &m_fences[i];
	}

private:
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderImageFinishedSemaphores;
	std::vector<VkFence> m_fences;

	Device * m_device;
};

Semaphore::Semaphore(Device * device) : m_device(device){
	m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_renderImageFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_fences.resize(MAX_FRAMES_IN_FLIGHT);

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		auto createInfo_0 = initialiser::createSemaphoreInfo();
		ASSERT(vkCreateSemaphore(m_device->getDevice(), &createInfo_0, nullptr, &m_imageAvailableSemaphores[i]), "Uable to create Semaphore");
		auto createInfo_1 = initialiser::createSemaphoreInfo();
		ASSERT(vkCreateSemaphore(m_device->getDevice(), &createInfo_1, nullptr, &m_renderImageFinishedSemaphores[i]), "Uable to create Semaphore");
		auto createInfo_2 = initialiser::createFenceInfo();
		ASSERT(vkCreateFence(m_device->getDevice(), &createInfo_2, nullptr, &m_fences[i]), "Unable to create fences");
	}
}

Semaphore::~Semaphore(){
	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(m_device->getDevice(), m_imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(m_device->getDevice(), m_renderImageFinishedSemaphores[i], nullptr);
		vkDestroyFence(m_device->getDevice(), m_fences[i], nullptr);
	}
}