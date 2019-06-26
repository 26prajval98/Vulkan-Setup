#pragma once

#include "Header.h"
#include "defines.h"
#include "device.h"
#include "initialiser.h"
#include "physicaldevice.h"

enum BUFFER_TYPE {
	VERTEX_BUFFER,
	INDEX_BUFFER,
	COMMAND_BUFFER,
	DEPTH_STENCIL_BUFFER
};

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

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {};
		
		attributeDescriptions.resize(2);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
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

class Buffer
{
public:
	Buffer(PhysicalDevice * physicalDevice, Device * device);
	~Buffer();
	BUFFER_TYPE virtual type() = 0;

protected:
	PhysicalDevice * m_physicalDevice;
	Device * m_device;

	VkBuffer m_buffer;
	VkDeviceMemory m_bufferMemory;

	VkBuffer getBuffer() {
		return m_buffer;
	}

	VkBuffer * pGetBuffer() {
		return &m_buffer;
	}

	void allocateAndMapBuffer(size_t bufferSize, void * bufferData) {
		void * m_mappedData;
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(m_device->getDevice(), m_buffer, &memoryRequirements);

		auto createInfo = initialiser::createMemoryAllocateInfo(memoryRequirements.size, findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_physicalDevice));
		ASSERT(vkAllocateMemory(m_device->getDevice(), &createInfo, nullptr, &m_bufferMemory), "Unable to allocate buffer memory");

		// Memory offset is 0
		vkBindBufferMemory(m_device->getDevice(), m_buffer, m_bufferMemory, 0);

		vkMapMemory(m_device->getDevice(), m_bufferMemory, 0, bufferSize, 0, &m_mappedData);
		memcpy(m_mappedData, bufferData, static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_device->getDevice(), m_bufferMemory);
	}

	void createBufferMemory(size_t bufferSize, void * verticesData) {
		auto createInfo = initialiser::createBufferInfo(bufferSize);
		ASSERT(vkCreateBuffer(m_device->getDevice(), &createInfo, nullptr, &m_buffer), "Unable to create vertex buffer");
		allocateAndMapBuffer(bufferSize, verticesData);
	}

private:

};

Buffer::Buffer(PhysicalDevice * physicalDevice, Device * device) : m_physicalDevice(physicalDevice), m_device(device)
{
}

Buffer::~Buffer(){
	vkDestroyBuffer(m_device->getDevice(), m_buffer, nullptr);
	vkFreeMemory(m_device->getDevice(), m_bufferMemory, nullptr);
}

class VertexBuffer : private Buffer
{
public:
	BUFFER_TYPE virtual type() { return VERTEX_BUFFER; };

	VertexBuffer(PhysicalDevice * physicalDevice, Device * device, std::vector<Vertex> vertices);
	~VertexBuffer();
	void setVertices(std::vector<Vertex>& vertices) {
		m_vertices = std::vector<Vertex>(vertices);
	}

	VkBuffer getVertexBuffer() {
		return getBuffer();
	}

	VkBuffer * pGetVertexBuffer() {
		return pGetBuffer();
	}

	uint32_t getNoVertices() {
		return static_cast<uint32_t>(m_vertices.size());
	}

private:
	std::vector<Vertex> m_vertices;
};

VertexBuffer::VertexBuffer(PhysicalDevice * physicalDevice, Device * device, std::vector<Vertex> vertices) : Buffer(physicalDevice, device) {
	setVertices(vertices);
	size_t bufferSize = sizeof(m_vertices[0]) * m_vertices.size();
	createBufferMemory(bufferSize, vertices.data());
}

VertexBuffer::~VertexBuffer() {

}

class IndexBuffer : private Buffer
{
public:
	IndexBuffer(PhysicalDevice * physicalDevice, Device * device, std::vector<uint32_t> indices);
	~IndexBuffer();

	BUFFER_TYPE type() { return INDEX_BUFFER; }

	VkBuffer getIndexBuffer() {
		return getBuffer();
	}

	VkBuffer * pGetIndexBuffer() {
		return pGetBuffer();
	}

	uint32_t getNoIndices() {
		return static_cast<uint32_t>(m_indices.size());
	}
private:
	std::vector<uint32_t> m_indices;
};

IndexBuffer::IndexBuffer(PhysicalDevice * physicalDevice, Device * device, std::vector<uint32_t> indices) : Buffer(physicalDevice, device), m_indices(indices)
{
	size_t bufferSize = sizeof(m_indices[0]) * m_indices.size();
	createBufferMemory(bufferSize, indices.data());
}

IndexBuffer::~IndexBuffer()
{
}