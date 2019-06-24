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

	void drawFrame() {
		uint32_t imageIndex;
		vkAcquireNextImageKHR(m_device->getDevice(), m_swapChain->getSwapChain(), std::numeric_limits<uint64_t>::max(), m_semaphore->getImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);
		VkSemaphore waitSemaphore[] = { m_semaphore->getImageAvailableSemaphore() };
		VkSemaphore signalSemaphore[] = { m_semaphore->getRenderImageFinishedSemaphore() };
		// When should wait semaphore be used
		VkPipelineStageFlags pipelineWaitStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo createInfo = initialiser::createSubmitInfo(waitSemaphore, signalSemaphore, pipelineWaitStageFlags, m_commandBuffer[imageIndex]);
		
		ASSERT(vkQueueSubmit(m_device->getGraphicsQueue(), 1, &createInfo, VK_NULL_HANDLE), "Failed to submit to command buffer");
	};

private:
	Window * m_window;
	Device * m_device;
	SwapChain * m_swapChain;
	Semaphore * m_semaphore;
	std::vector<VkCommandBuffer> m_commandBuffer;
};

Draw::Draw(Window * window, Device * device, SwapChain * swapChain, Semaphore * semaphore, std::vector<VkCommandBuffer>& commandBuffer) : m_window(window), m_device(device),
																					m_swapChain(swapChain), m_semaphore(semaphore), 
																					m_commandBuffer(commandBuffer){	
	while (!glfwWindowShouldClose(window->getWindow()))
	{
		glfwPollEvents();
		drawFrame();
	}
}

Draw::~Draw()
{
}