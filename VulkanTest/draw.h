#pragma once

#include "Header.h"
#include "window.h"
#include "defines.h"
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
	uint32_t m_currentFrameNumber;
	//bool x;

	std::vector<VkCommandBuffer> m_commandBuffer;
	VkSwapchainKHR m_pSwapChain[1];

	void drawFrame() {
		uint32_t imageIndex;

		// We will only have to set-up and reset fences manually
		// It will wait for ever without the flagged bits
		// 1 way to overcome is to hold a bool but with that wyou cannot close the window so VK_FENCE_CREATE_SIGNALED_BIT flag is used i.e create fences in signaled state
		// if(x)

		vkWaitForFences(m_device->getDevice(), 1, m_semaphore->pGetFence(m_currentFrameNumber), VK_TRUE, std::numeric_limits<uint64_t>::max());
		vkResetFences(m_device->getDevice(), 1, m_semaphore->pGetFence(m_currentFrameNumber));

		// If you get vk_error_validation_failed_ext it is due to not getting to know the semaphores properly
		vkAcquireNextImageKHR(m_device->getDevice(), m_swapChain->getSwapChain(), std::numeric_limits<uint64_t>::max(), m_semaphore->getImageAvailableSemaphore(m_currentFrameNumber), VK_NULL_HANDLE, &imageIndex);
		// When should wait semaphore be used
		VkPipelineStageFlags pipelineWaitStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo createInfo_0 = initialiser::createSubmitInfo(m_semaphore->pGetImageAvailableSemaphore(m_currentFrameNumber), m_semaphore->pGetRenderImageFinishedSemaphore(m_currentFrameNumber), pipelineWaitStageFlags, m_commandBuffer[imageIndex]);

		ASSERT(vkQueueSubmit(m_device->getGraphicsQueue(), 1, &createInfo_0, m_semaphore->getFence(m_currentFrameNumber)), "Failed to submit to command buffer");

		VkPresentInfoKHR createInfo_1 = initialiser::createPresentInfo(m_semaphore->pGetRenderImageFinishedSemaphore(m_currentFrameNumber), m_pSwapChain, imageIndex);

		vkQueuePresentKHR(m_device->getPresentQueue(), &createInfo_1);

		++m_currentFrameNumber;
		m_currentFrameNumber %= MAX_FRAMES_IN_FLIGHT;
		//x = true;
	};
};

Draw::Draw(Window * window, Device * device, SwapChain * swapChain, Semaphore * semaphore, std::vector<VkCommandBuffer>& commandBuffer) : m_window(window),
m_device(device), m_swapChain(swapChain), m_semaphore(semaphore), m_currentFrameNumber(0), m_commandBuffer(commandBuffer) {

	m_pSwapChain[0] = { swapChain->getSwapChain() };
	//x = false;
	while (!glfwWindowShouldClose(window->getWindow()))
	{
		glfwPollEvents();
		drawFrame();

		// Works but reduces performance
		// vkQueueWaitIdle(m_device->getPresentQueue());
	}

	// Last constant buffers and all close
	vkDeviceWaitIdle(m_device->getDevice());
}

Draw::~Draw()
{
}