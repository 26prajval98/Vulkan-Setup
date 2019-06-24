#pragma once

#include "initialiser.h"
#include "shaders.h"
#include "swapchain.h"

class Pipeline
{
public:
	Pipeline(Shaders * shaders, SwapChain * swapChain);
	~Pipeline();

private:
	Shaders * m_shaders;
};

Pipeline::Pipeline(Shaders * shaders, SwapChain * swapChain) : m_shaders(shaders)
{
	VkPipelineShaderStageCreateInfo vertexShaderInfo = initialiser::createPipelineShaderStageInfo(m_shaders->getVertexShaderModule(), VK_SHADER_STAGE_VERTEX_BIT);
	VkPipelineShaderStageCreateInfo fragmentShaderInfo = initialiser::createPipelineShaderStageInfo(m_shaders->getFragmentShaderModule());

	auto pipelineVertexInput = initialiser::createPipelineVertexInputStateInfo();

	auto pipelineInputAssemblyInfo = initialiser::createPipelineInputAssemblyInfo();

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

	auto pipelineRasterizationInfo = initialiser::createPipelineRasterizationStateInfo();

	auto pipelineMultisampleInfo = initialiser::createPipelineMultisampleStateInfo();

	auto pipelineDepthStencilInfo = initialiser::createPipelineDepthStencilStateInfo();

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

	VkDynamicState pDynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};
	auto pipelineDynamicInfo = initialiser::createPipelineDynamicStateInfo(pDynamicStates);

	auto pipelineLayoutInfo = initialiser::createPipelineLayoutInfo();


}

Pipeline::~Pipeline()
{
}