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

	VkShaderModule getVertexShader() {
		return m_vertexShader;
	}

	VkShaderModule getFragmentShader() {
		return m_fragmentShader;
	}

private:
	VkShaderModule m_vertexShader;
	VkShaderModule m_fragmentShader;

	Device * m_device;
};

Shaders::Shaders(Device * device) : m_device(device)
{
	auto createInfo_0 = initialiser::createShaderModuleInfo(readFile("vert.spv"));
	ASSERT(vkCreateShaderModule(m_device->getDevice(), &createInfo_0, nullptr, &m_vertexShader), "UNABLE TO CREATE SHADER MODULE");

	auto createInfo_1 = initialiser::createShaderModuleInfo(readFile("frag.spv"));
	ASSERT(vkCreateShaderModule(m_device->getDevice(), &createInfo_1, nullptr, &m_fragmentShader), "UNABLE TO CREATE SHADER MODULE");
}

Shaders::~Shaders()
{
	vkDestroyShaderModule(m_device->getDevice(), m_vertexShader, nullptr);
	vkDestroyShaderModule(m_device->getDevice(), m_fragmentShader, nullptr);
}