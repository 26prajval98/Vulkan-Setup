#pragma once

#include "Header.h"
#include "defines.h"
#include "device.h"
#include "initialiser.h"
#include "swapchain.h"

class RenderPass
{
public:
	RenderPass(Device * device, SwapChain * swapChain);
	~RenderPass();

private:
	VkRenderPass m_renderPass;
	std::vector<VkAttachmentDescription>m_attachmentDescriptions;
	std::vector<std::vector<VkAttachmentReference>> m_attachmentReferences;
	std::vector<VkSubpassDescription> m_subpassDescriptions;

	Device * m_device;
	SwapChain * m_swapChain;
};

RenderPass::RenderPass(Device * device, SwapChain * swapChain) : m_device(device), m_swapChain(swapChain)
{
	// Array of attachmentDescription are added to the renderpass
	// A RenderPass several subpass(es)
	// Each subpass has one or more attachments which refers to index of attachmentDescription array

	m_attachmentDescriptions = {};

	VkAttachmentDescription colorAttachmentDescription = {};
	colorAttachmentDescription.format = m_swapChain->getSurfaceFormat().format;
	colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	m_attachmentDescriptions.push_back(colorAttachmentDescription);

	// for 1st subpass color attachment reference
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_attachmentReferences.resize(1);

	m_attachmentReferences[0].push_back(colorAttachmentRef);
	// Layout : Initial (Undefined) -> middle (Optimal) -> final (present in swapchain)

	/*
		attachments : 1-2-3-4 (binding with renderpass)
		attachment refs: 
			1-2- (binding with subpass_1)
			3-	 (binding with subpass_2)
			4-	 (binding with subpass_3)
										
	*/

	// Create A Single subpass
	m_subpassDescriptions.resize(1);
	VkSubpassDescription subpassDescription = {};
	// Vulkan currently supports graphics subpass but might support compute ones in future
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = static_cast<uint32_t>(m_attachmentReferences[0].size());
	subpassDescription.pColorAttachments = m_attachmentReferences[0].data();
	m_subpassDescriptions.push_back(subpassDescription);

	VkRenderPassCreateInfo renderPassInfo = initialiser::createRenderPassInfo(m_attachmentDescriptions, m_subpassDescriptions);
	ASSERT(vkCreateRenderPass(m_device->getDevice(), &renderPassInfo, nullptr, &m_renderPass), "UNABLE TO CREATE RENDERPASS");
}

RenderPass::~RenderPass()
{
	vkDestroyRenderPass(m_device->getDevice(), m_renderPass, nullptr);
}