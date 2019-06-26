#pragma once

#include "Header.h"
#include "initialiser.h"
#include "pipeline.h"
#include "swapchain.h"

class Descriptors
{
public:
	Descriptors(Device * device, SwapChain * swapChain, Pipeline * pipeline);
	~Descriptors();

private:
	VkDescriptorPool m_descriptorPool;
	std::vector<VkDescriptorSet> m_descriptorSets;

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
		vkAllocateDescriptorSets(m_device->getDevice(), &createInfo, m_descriptorSets.data());
	}
};

Descriptors::Descriptors(Device * device, SwapChain * swapChain, Pipeline * pipeline) : m_device(device), m_swapChain(swapChain), m_pipeline(pipeline)
{
	createDescriptorPool();
	createDescriptorSets();
}

Descriptors::~Descriptors()
{
	vkDestroyDescriptorPool(m_device->getDevice(), m_descriptorPool, nullptr);
}