#pragma once

#include "Header.h"
#include "device.h"
#include "initialiser.h"
#include "shaders.h"
#include "swapchain.h"
#include "renderpass.h"
//#include "buffers.h"

class Pipeline
{
public:
	Pipeline(Device * device, RenderPass * renderPass, Shaders * shaders, SwapChain * swapChain, std::array<VkVertexInputAttributeDescription, 2>&attributeDescription, VkVertexInputBindingDescription &bindingDesciption);
	~Pipeline();

	VkPipeline getGraphicsPipeline() {
		return m_pipeline;
	}

private:
	initialiser::PipelineStages pipelineStages = {};
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_pipeline;
	std::vector<VkGraphicsPipelineCreateInfo> m_pipelineLayoutInfos;

	Device * m_device;
	RenderPass * m_renderPass;
	Shaders * m_shaders;
	SwapChain * m_swapChain;
	//std::vector<VkVertexInputAttributeDescription> attributeDescription;
	//VkVertexInputBindingDescription bindingDescription;
};

Pipeline::Pipeline(Device * device, RenderPass * renderPass, Shaders * shaders, SwapChain * swapChain, std::array<VkVertexInputAttributeDescription, 2>&attributeDescription, VkVertexInputBindingDescription &bindingDesciption) : m_shaders(shaders), m_renderPass(renderPass), m_device(device), m_swapChain(swapChain)
{
	VkPipelineShaderStageCreateInfo vertexShaderInfo = initialiser::createPipelineShaderStageInfo(m_shaders->getVertexShaderModule(), VK_SHADER_STAGE_VERTEX_BIT);
	VkPipelineShaderStageCreateInfo fragmentShaderInfo = initialiser::createPipelineShaderStageInfo(m_shaders->getFragmentShaderModule(), VK_SHADER_STAGE_FRAGMENT_BIT);
	VkPipelineShaderStageCreateInfo pShaderStage[] = { vertexShaderInfo, fragmentShaderInfo };
	pipelineStages.pShaderStage = pShaderStage;

	auto pipelineVertexInput = initialiser::createPipelineVertexInputStateInfo(attributeDescription, bindingDesciption);
	pipelineStages.pipelineVertexInput = pipelineVertexInput;

	auto pipelineInputAssemblyInfo = initialiser::createPipelineInputAssemblyInfo();
	pipelineStages.pipelineInputAssemblyInfo = pipelineInputAssemblyInfo;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChain->getSwapChainExtent().width;
	viewport.height = (float)swapChain->getSwapChainExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain->getSwapChainExtent();
	auto pipelineViewPortInfo = initialiser::createPipelineViewportStateInfo(viewport, scissor);
	pipelineStages.pipelineViewPortInfo = pipelineViewPortInfo;

	auto pipelineRasterizationInfo = initialiser::createPipelineRasterizationStateInfo();
	pipelineStages.pipelineRasterizationInfo = pipelineRasterizationInfo;

	auto pipelineMultisampleInfo = initialiser::createPipelineMultisampleStateInfo();
	pipelineStages.pipelineMultisampleInfo = pipelineMultisampleInfo;

	auto pipelineDepthStencilInfo = initialiser::createPipelineDepthStencilStateInfo();
	pipelineStages.pipelineDepthStencilInfo = pipelineDepthStencilInfo;

	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachment = {};
	pipelineColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	pipelineColorBlendAttachment.blendEnable = VK_FALSE;
	pipelineColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	pipelineColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	pipelineColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	pipelineColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	pipelineColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	pipelineColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
	auto pipelineColorBlendInfo = initialiser::createPipelineColorBlendStateInfo(pipelineColorBlendAttachment);
	pipelineStages.pipelineColorBlendInfo = pipelineColorBlendInfo;

	VkDynamicState pDynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};
	auto pipelineDynamicInfo = initialiser::createPipelineDynamicStateInfo(pDynamicStates);
	pipelineStages.pipelineDynamicInfo = pipelineDynamicInfo;

	auto createInfo_0 = initialiser::createPipelineLayoutInfo();
	ASSERT(vkCreatePipelineLayout(device->getDevice(), &createInfo_0, nullptr, &m_pipelineLayout), "Unable to create pipeline layout");
	
	m_pipelineLayoutInfos = {};
	// 0th index in subpass of renderpass (subpassIndex)
	auto createInfo_1 = initialiser::createGrpahicsPipilineInfo(pipelineStages, m_pipelineLayout, renderPass->getRenderPass(), 0);
	m_pipelineLayoutInfos.push_back(createInfo_1);
	ASSERT(vkCreateGraphicsPipelines(device->getDevice(), VK_NULL_HANDLE, U(m_pipelineLayoutInfos.size()), m_pipelineLayoutInfos.data(), nullptr, &m_pipeline), "Unable to create graphics pipeline");
}

Pipeline::~Pipeline()
{
	vkDestroyPipeline(m_device->getDevice(), m_pipeline, nullptr);
	vkDestroyPipelineLayout(m_device->getDevice(), m_pipelineLayout, nullptr);
}
