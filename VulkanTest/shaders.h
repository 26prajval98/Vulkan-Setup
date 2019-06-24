#pragma once

#include "Header.h"
#include "device.h"
#include "initialiser.h"


class Shaders
{
public:
	Shaders(Device * device);
	~Shaders();
	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkShaderModule getVertexShaderModule() {
		return m_vertexShaderModule;
	}

	VkShaderModule getFragmentShaderModule() {
		return m_fragmentShaderModule;
	}

private:
	VkShaderModule m_vertexShaderModule;
	VkShaderModule m_fragmentShaderModule;

	Device * m_device;
};

Shaders::Shaders(Device * device) : m_device(device)
{
	auto createInfo_0 = initialiser::createShaderModuleInfo(readFile("vert.spv"));
	ASSERT(vkCreateShaderModule(m_device->getDevice(), &createInfo_0, nullptr, &m_vertexShaderModule), "UNABLE TO CREATE SHADER MODULE");

	auto createInfo_1 = initialiser::createShaderModuleInfo(readFile("frag.spv"));
	ASSERT(vkCreateShaderModule(m_device->getDevice(), &createInfo_1, nullptr, &m_fragmentShaderModule), "UNABLE TO CREATE SHADER MODULE");
}

Shaders::~Shaders()
{
	vkDestroyShaderModule(m_device->getDevice(), m_vertexShaderModule, nullptr);
	vkDestroyShaderModule(m_device->getDevice(), m_fragmentShaderModule, nullptr);
}