#pragma once

#include "Header.h"
#include "window.h"
#include "device.h"
#include "swapchain.h"
#include "semaphores.h"

class Draw
{
public:
	Draw(Window * window, Device * device, SwapChain * swapChain, Semaphore * semaphore, std::vector<VkCommandBuffer>& commandBuffer);
	~Draw();

private:
	Window * m_window;
	Device * m_device;
	SwapChain * m_swapChain;
	Semaphore * m_semaphore;

	std::vector<VkCommandBuffer> m_commandBuffer;
	VkSemaphore m_waitSemaphore[1];
	VkSemaphore m_signalSemaphore[1];
	VkSwapchainKHR m_pSwapChain[1];

	void drawFrame() {
		uint32_t imageIndex;

		// If you get vk_error_validation_failed_ext it is due to not getting to know the semaphores properly
		vkAcquireNextImageKHR(m_device->getDevice(), m_swapChain->getSwapChain(), std::numeric_limits<uint64_t>::max(), m_waitSemaphore[0], VK_NULL_HANDLE, &imageIndex);
		// When should wait semaphore be used
		VkPipelineStageFlags pipelineWaitStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo createInfo_0 = initialiser::createSubmitInfo(m_waitSemaphore, m_signalSemaphore, pipelineWaitStageFlags, m_commandBuffer[imageIndex]);

		ASSERT(vkQueueSubmit(m_device->getGraphicsQueue(), 1, &createInfo_0, VK_NULL_HANDLE), "Failed to submit to command buffer");

		VkPresentInfoKHR createInfo_1 = initialiser::createPresentInfo(m_signalSemaphore, m_pSwapChain, imageIndex);

		vkQueuePresentKHR(m_device->getPresentQueue(), &createInfo_1);
	};
};

Draw::Draw(Window * window, Device * device, SwapChain * swapChain, Semaphore * semaphore, std::vector<VkCommandBuffer>& commandBuffer) : m_window(window), m_device(device),
m_swapChain(swapChain), m_semaphore(semaphore),
m_commandBuffer(commandBuffer) {
	m_waitSemaphore[0] = m_semaphore->getImageAvailableSemaphore();
	m_signalSemaphore[0] = m_semaphore->getRenderImageFinishedSemaphore();
	m_pSwapChain[0] = { swapChain->getSwapChain() };
	while (!glfwWindowShouldClose(window->getWindow()))
	{
		glfwPollEvents();
		drawFrame();
		vkQueueWaitIdle(m_device->getPresentQueue());
	}

	// Last constant buffers and all close
	vkDeviceWaitIdle(m_device->getDevice());
}

Draw::~Draw()
{
}