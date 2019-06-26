#pragma once

#include "Header.h"
#include "defines.h"
#include "device.h"
#include "initialiser.h"
#include "swapchain.h"
//#include "buffers.h"

struct CommandDetails {
	std::vector<VkCommandBuffer> * pCommandBuffer;
	std::vector<VkFramebuffer> frameBuffer;
	VkPipeline graphicsPipeline;
	VkDeviceSize offset;
	VkBuffer * pVertexBuffer;
	VkBuffer indexBuffer;
	uint32_t verticesCount;
	uint32_t indicesCount;
};

class RenderPass
{
public:
	RenderPass(Device * device, SwapChain * swapChain);
	~RenderPass();

	VkRenderPass getRenderPass() {
		return m_renderPass;
	}

	void command(CommandDetails &commandDetails) {
		unsigned int i = 0;
		for (auto& cb : *(commandDetails.pCommandBuffer)) {
			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			auto createInfo_0 = initialiser::createCommandBeginInfo();
			ASSERT(vkBeginCommandBuffer(cb, &createInfo_0), "Command Buffer unable to begin");
			auto createInfo_1 = initialiser::createRenderPassBeginInfo(commandDetails.frameBuffer[i], m_swapChain->getSwapChainExtent(), m_renderPass, clearColor);

			// VK_SUBPASS_CONTENTS_INLINE means execute from primary command buffet
			vkCmdBeginRenderPass(cb, &createInfo_1, VK_SUBPASS_CONTENTS_INLINE);
			// VK_PIPELINE_BIND_POINT_GRAPHICS is graphics and 1 for compute

			vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, commandDetails.graphicsPipeline);
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(cb, 0, 1, commandDetails.pVertexBuffer, offsets);
			// cb, vertices, instanceCount = 1 if not instance rendering, firstVertex, firstInstance
			vkCmdBindIndexBuffer(cb, commandDetails.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
			vkCmdDraw(cb, commandDetails.verticesCount, 1, 0, 0);

			vkCmdEndRenderPass(cb);
			ASSERT(vkEndCommandBuffer(cb), "Failed to record commands");
			++i;
		}
	}

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
	m_subpassDescriptions = {};
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