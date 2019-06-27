#pragma once

#include "Header.h"
#include "initialiser.h"
#include "pipeline.h"
#include "swapchain.h"
#include "buffers.h"

class Descriptors
{
public:
	Descriptors(Device * device, SwapChain * swapChain, Pipeline * pipeline, UniformBuffers * uniformBuffers);
	~Descriptors();
	
	VkDescriptorSet * pGetDescriptorSet(uint32_t index) {
		return &m_descriptorSets[index];
	}

private:
	VkDescriptorPool m_descriptorPool;
	std::vector<VkDescriptorSet> m_descriptorSets;
	UniformBuffers * m_uniformBuffers;

	Device * m_device;
	SwapChain * m_swapChain;
	Pipeline * m_pipeline;

	void createDescriptorPool() {
		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = U(m_swapChain->getImages().size());
		auto createInfo = initialiser::createDescriptorPoolInfo(poolSize, U(m_swapChain->getImages().size()));
		ASSERT(vkCreateDescriptorPool(m_device->getDevice(), &createInfo, nullptr, &m_descriptorPool), "Unable to create descriptor pool");
	}

	void createDescriptorSets() {
		std::vector <VkDescriptorSetLayout> descriptorSetLayouts(m_swapChain->getImages().size(), m_pipeline->getDescriptorSetLayout());
		VkDescriptorSetAllocateInfo createInfo = initialiser::createDescriptorSetAllocateInfo(m_descriptorPool, descriptorSetLayouts, m_swapChain->getImages().size());
		m_descriptorSets.resize(m_swapChain->getImages().size());
		ASSERT(vkAllocateDescriptorSets(m_device->getDevice(), &createInfo, m_descriptorSets.data()), "Not able to form descriptor sets");

		for (uint32_t i = 0; i < m_swapChain->getImages().size(); ++i) {
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = m_uniformBuffers->getBuffer(i);
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional
			vkUpdateDescriptorSets(m_device->getDevice(), 1, &descriptorWrite, 0, nullptr);
		}		
	}
};

Descriptors::Descriptors(Device * device, SwapChain * swapChain, Pipeline * pipeline, UniformBuffers * uniformBuffers) : m_device(device), m_swapChain(swapChain), m_pipeline(pipeline), m_uniformBuffers(uniformBuffers)
{
	createDescriptorPool();
	createDescriptorSets();
}

Descriptors::~Descriptors()
{
	vkDestroyDescriptorPool(m_device->getDevice(), m_descriptorPool, nullptr);
}