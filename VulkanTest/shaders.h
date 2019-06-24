#pragma once

#include "Header.h"
#include "device.h"
#include "initialiser.h"
#include "readfile.h"

class Shaders
{
public:
	Shaders(Device * device);
	~Shaders();
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