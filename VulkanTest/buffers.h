#pragma once

#include "glm/glm.hpp"

#include "Header.h"
#include "defines.h"
#include "device.h"
#include "initialiser.h"
#include "physicaldevice.h"

struct Vertex {
	glm::vec2 position;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};

		// vertex attribute addressing is a function of the vertex index
		// We are not doing any instance rendering now
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescription() {
		std::vector<VkVertexInputAttributeDescription> attributeDescription(2);

		attributeDescription[0].binding = 0;
		attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription[0].offset = offsetof(Vertex, position);
		attributeDescription[0].location = 0;
		attributeDescription[1].binding = 0;
		attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[1].offset = offsetof(Vertex, color);
		attributeDescription[1].location = 1;
		return attributeDescription;
	}
};

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags memoryPropertyFlag, PhysicalDevice * physicalDevice) {
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice->getPhysicalDevice(), &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlag) == memoryPropertyFlag)
			return i;
	}
	throw std::runtime_error("Suitable memory type not found");
}

class VertexBuffer
{
public:
	VertexBuffer(PhysicalDevice * physicalDevice, Device * device, std::vector<Vertex>& vertices);
	~VertexBuffer();
	void setVertices(std::vector<Vertex>& vertices) {
		m_vertices = std::vector<Vertex>(vertices);
	}

	VkBuffer getVertexBuffer() {
		return m_vertexBuffer;
	}

	uint32_t getNoVertices() {
		return static_cast<uint32_t>(m_vertices.size());
	}

private:
	PhysicalDevice * m_physicalDevice;
	Device * m_device;
	void * m_mappedData;

	std::vector<Vertex> m_vertices;
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	void allocateAndMapBuffer(uint32_t vertexBufferSize) {
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(m_device->getDevice(), m_vertexBuffer, &memoryRequirements);

		auto createInfo_1 = initialiser::createMemoryAllocateInfo(memoryRequirements.size, findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_physicalDevice));
		ASSERT(vkAllocateMemory(m_device->getDevice(), &createInfo_1, nullptr, &m_vertexBufferMemory), "Unable to allocate vertex buffer memory");

		// Memory offset is 0
		vkBindBufferMemory(m_device->getDevice(), m_vertexBuffer, m_vertexBufferMemory, 0);

		vkMapMemory(m_device->getDevice(), m_vertexBufferMemory, 0, vertexBufferSize, 0, &m_mappedData);
		memcpy(m_mappedData, m_vertices.data(), static_cast<size_t>(vertexBufferSize));
		vkUnmapMemory(m_device->getDevice(), m_vertexBufferMemory);
	}
};

VertexBuffer::VertexBuffer(PhysicalDevice * physicalDevice, Device * device, std::vector<Vertex>& vertices) : m_physicalDevice(physicalDevice), m_device(device) {
	setVertices(vertices);
	uint32_t vertexBufferSize = sizeof(m_vertices[0]) * m_vertices.size();
	auto createInfo_0 = initialiser::createVertexBufferInfo(vertexBufferSize);
	ASSERT(vkCreateBuffer(m_device->getDevice(), &createInfo_0, nullptr, &m_vertexBuffer), "Unable to create vertex buffer");
	
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(m_device->getDevice(), m_vertexBuffer, &memoryRequirements);

	auto createInfo_1 = initialiser::createMemoryAllocateInfo(memoryRequirements.size, findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_physicalDevice));
	ASSERT(vkAllocateMemory(m_device->getDevice(), &createInfo_1, nullptr, &m_vertexBufferMemory), "Unable to allocate vertex buffer memory");

	// Memory offset is 0
	vkBindBufferMemory(m_device->getDevice(), m_vertexBuffer, m_vertexBufferMemory, 0);

	vkMapMemory(m_device->getDevice(), m_vertexBufferMemory, 0, vertexBufferSize, 0, &m_mappedData);
	memcpy(m_mappedData, m_vertices.data(), (size_t)(vertexBufferSize));
	vkUnmapMemory(m_device->getDevice(), m_vertexBufferMemory);

	//allocateAndMapBuffer(vertexBufferSize);
}

VertexBuffer::~VertexBuffer() {
	vkDestroyBuffer(m_device->getDevice(), m_vertexBuffer, nullptr);
	vkFreeMemory(m_device->getDevice(), m_vertexBufferMemory, nullptr);
}