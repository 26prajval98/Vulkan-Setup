#pragma once

#include "Header.h"
#include "defines.h"
#include "device.h"
#include "renderpass.h"
#include "swapchain.h"


class FrameBuffer
{
public:
	FrameBuffer(Device * device, SwapChain * swapChain, RenderPass * renderPass);
	~FrameBuffer();

	std::vector<VkFramebuffer> getFrameBuffer() {
		return m_frameBuffer;
	}

private:
	std::vector<VkFramebuffer> m_frameBuffer;

	Device * m_device;
	SwapChain * m_swapChain;
	RenderPass * m_renderPass;
};

FrameBuffer::FrameBuffer(Device * device, SwapChain * swapChain, RenderPass * renderPass) : m_device(device), m_swapChain(swapChain), m_renderPass(renderPass)
{
	m_frameBuffer.resize(swapChain->getImageViews().size());

	int i = 0;
	for (auto& fb : m_frameBuffer) {
		VkImageView imageViewAttachments[] = { swapChain->getImageViews()[i] };
		auto createInfo = initialiser::createFrameBufferInfo(imageViewAttachments, m_renderPass->getRenderPass(), m_swapChain->getSwapChainExtent());
		ASSERT(vkCreateFramebuffer(m_device->getDevice(), &createInfo, nullptr, &m_frameBuffer[i]), "Unable to create framebuffer");
		++i;
	}
}

FrameBuffer::~FrameBuffer()
{
	for (auto& fb : m_frameBuffer) {
		vkDestroyFramebuffer(m_device->getDevice(), fb, nullptr);
	}
}