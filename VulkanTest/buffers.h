#pragma once

#include "Header.h"
#include "defines.h"
#include "device.h"
#include "initialiser.h"
#include "physicaldevice.h"
#include "swapchain.h"

enum BUFFER_TYPE {
	VERTEX_BUFFER,
	INDEX_BUFFER,
	UNIFORM_BUFFER,
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

struct UniformBufferObject {
	alignas(16)glm::mat4 model;
	alignas(16)glm::mat4 view;
	alignas(16)glm::mat4 proj;
} ;

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

	void mapMemory(size_t bufferSize, void * bufferData) {
		void * m_mappedData;
		vkMapMemory(m_device->getDevice(), m_bufferMemory, 0, bufferSize, 0, &m_mappedData);
		memcpy(m_mappedData, bufferData, static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_device->getDevice(), m_bufferMemory);
	}

	void allocateMemory() {
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(m_device->getDevice(), m_buffer, &memoryRequirements);
		auto memoryType = findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_physicalDevice);
		auto createInfo_1 = initialiser::createMemoryAllocateInfo(memoryRequirements.size, memoryType);
		ASSERT(vkAllocateMemory(m_device->getDevice(), &createInfo_1, nullptr, &m_bufferMemory), "Unable to allocate buffer memory");
		// Memory offset is 0
		vkBindBufferMemory(m_device->getDevice(), m_buffer, m_bufferMemory, 0);
	}

	void createBuffer(size_t bufferSize, VkBufferUsageFlags usageFlags) {
		auto createInfo = initialiser::createBufferInfo(bufferSize, usageFlags);
		ASSERT(vkCreateBuffer(m_device->getDevice(), &createInfo, nullptr, &m_buffer), "Unable to create vertex buffer");
	}

	void createBufferMemoryWithoutMap(size_t bufferSize, VkBufferUsageFlags usageFlags) {
		createBuffer(bufferSize, usageFlags);
		allocateMemory();
	}

	void createBufferMemory(size_t bufferSize, void * data, VkBufferUsageFlags usageFlags) {
		createBuffer(bufferSize, usageFlags);
		allocateMemory();
		mapMemory(bufferSize, data);
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
	size_t bufferSize = sizeof(Vertex) * m_vertices.size();
	createBufferMemory(bufferSize, vertices.data(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
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
	size_t bufferSize = sizeof(uint32_t) * m_indices.size();
	createBufferMemory(bufferSize, indices.data(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}

IndexBuffer::~IndexBuffer()
{
}

class UniformBuffers
{
public:
	UniformBuffers(PhysicalDevice * physicalDevice, Device * device, SwapChain * swapChain);
	~UniformBuffers();

	VkBuffer getBuffer(uint32_t index) {
		return uniformBuffers[index];
	}

	void updateUniformBuffer(uint32_t currentImage) {
		auto device = m_device->getDevice();

		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo = {};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), m_swapChain->getSwapChainExtent().width / (float)m_swapChain->getSwapChainExtent().height, 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
	}

	void createBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, m_physicalDevice);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

private:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	Device * m_device;
	PhysicalDevice * m_physicalDevice;
	SwapChain * m_swapChain;
};

UniformBuffers::UniformBuffers(PhysicalDevice * physicalDevice, Device * device, SwapChain * swapChain) : m_device(device), m_physicalDevice(physicalDevice), m_swapChain(swapChain)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(m_swapChain->getImages().size());
	uniformBuffersMemory.resize(m_swapChain->getImages().size());

	for (size_t i = 0; i < m_swapChain->getImages().size(); i++) {
		createBuffer(m_device->getDevice(), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

UniformBuffers::~UniformBuffers()
{
}

//
//class UniformBuffer : private Buffer
//{
//public:
//	UniformBuffer(PhysicalDevice * physicalDevice, Device * device, SwapChain * swapChain);
//	~UniformBuffer();
//	
//	BUFFER_TYPE type() { return UNIFORM_BUFFER; }
//	void updateUniformBuffer() {
//		getUbo();
//		mapMemory(sizeof(UniformBufferObject), &m_uniformBuffer);
//	}
//
//	VkBuffer getUniformBuffer() {
//		return getBuffer();
//	}
//
//private:
//	SwapChain * m_swapChain;
//	UniformBufferObject m_uniformBuffer;
//
//	void getUbo() {
//		static auto startTime = std::chrono::high_resolution_clock::now();
//		auto currentTime = std::chrono::high_resolution_clock::now();
//		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//		
//		m_uniformBuffer.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//
//		m_uniformBuffer.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//
//		m_uniformBuffer.proj = glm::perspective(glm::radians(45.0f), m_swapChain->getSwapChainExtent().width / (float)m_swapChain->getSwapChainExtent().height, 0.1f, 10.0f);
//		m_uniformBuffer.proj[1][1] *= -1;
//	}
//
//};
//
//UniformBuffer::UniformBuffer(PhysicalDevice * physicalDevice, Device * device, SwapChain * swapChain) : Buffer(physicalDevice, device), m_swapChain(swapChain)
//{
//	size_t bufferSize = sizeof(UniformBufferObject);
//	createBufferMemoryWithoutMap(sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
//}
//
//UniformBuffer::~UniformBuffer()
//{
//}
//
//class UniformBuffers
//{
//public:
//	UniformBuffers(PhysicalDevice * physicalDevice, Device * device, SwapChain * swapChain);
//	~UniformBuffers();
//
//	VkBuffer getBuffer(uint32_t index) {
//		return m_uniformBuffers[index]->getUniformBuffer();
//	}
//
//	void updateUniformBuffer(uint32_t index) {
//		m_uniformBuffers[index]->updateUniformBuffer();
//	}
//
//private:
//	PhysicalDevice * m_physicalDevice;
//	Device * m_device;
//	SwapChain * m_swapChain;
//	std::vector<UniformBuffer *> m_uniformBuffers;
//};
//
//UniformBuffers::UniformBuffers(PhysicalDevice * physicalDevice, Device * device, SwapChain * swapChain) : m_device(device), m_physicalDevice(physicalDevice), m_swapChain(swapChain), m_uniformBuffers({})
//{
//	for (uint32_t i = 0; i < m_swapChain->getImageViews().size(); i++) {
//		auto x = new UniformBuffer(m_physicalDevice, m_device, m_swapChain);
//		m_uniformBuffers.push_back(x);
//	}
//}
//
//UniformBuffers::~UniformBuffers()
//{
//	for (auto& m_uniformBuffer : m_uniformBuffers) {
//		delete(m_uniformBuffer);
//	}
//}