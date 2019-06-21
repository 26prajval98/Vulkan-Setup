#pragma once

#include "Header.h"

class Instance
{
public:
	Instance();
	~Instance();
	VkInstance getInstance() {
		return m_instance;
	}

private:
	VkInstance m_instance;
	std::vector<const char *>extensions;
	std::vector<const char *>layers;
	void cleanUp() {
		vkDestroyInstance(m_instance, nullptr);
	}
};

Instance::Instance()
{
	VkResult res;
	// Instance creation
	VkApplicationInfo appInfo = initialiser::createAppInfo();
	VkInstanceCreateInfo createInfo = initialiser::createInfo(appInfo, extensions, layers);
	res = vkCreateInstance(&createInfo, nullptr, &m_instance);

	assert(VK_SUCCESS == res && "INSTANCE ERROR");
}

Instance::~Instance()
{
	cleanUp();
}