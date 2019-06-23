#pragma once

#include "Header.h"
#include "initialiser.h"

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
	layers.push_back("VK_LAYER_LUNARG_standard_validation");
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	//const char* extensions_array[] = {
	//	"VK_EXT_debug_report",
	//	"VK_KHR_surface",
	//};

	extensions = std::vector<const char *>(glfwExtensions, glfwExtensions + glfwExtensionCount);
	
	extensions.push_back("VK_EXT_debug_report");

	VkApplicationInfo appInfo = initialiser::createAppInfo();
	VkInstanceCreateInfo createInfo = initialiser::createInstanceInfo(appInfo, extensions, layers);
	res = vkCreateInstance(&createInfo, nullptr, &m_instance);

	assert(VK_SUCCESS == res && "INSTANCE ERROR");
}

Instance::~Instance()
{
	cleanUp();
}