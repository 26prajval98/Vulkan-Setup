#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <cstring>

#define DEBUG

const int width = 800;
const int height = 600;


// All of the useful standard validation is bundled into a layer included in the SDK that is known as VK_LAYER_KHRONOS_validation
const char * validLayers[] = {
	"VK_LAYER_KHRONOS_validation"
};

const int validLayersCount = 1;

// Enable validation only in debug mode
#ifdef DEBUG
const bool enableValidationLayer = true;
#else
const bool enableValidationLayer = false;
#endif // DEBUG


class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;
	VkInstance instance;

	std::vector<const char *> getRequiredExtensions() {
		unsigned int glfwExtensionCount = 0;
		const char ** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayer) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool checkValidationLayerSupport() {

		// Get all the validation layer support

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		// check if all validation layers are present
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (int i = 0; i < validLayersCount; i++)
		{
			bool layerFound = false;
			for (auto& availableLayer : availableLayers) {
				std::cout << validLayers[i] << availableLayer.layerName << std::endl;
				if (strcmp(validLayers[i], availableLayer.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}
			if (!layerFound)
				return false;
		}
		return true;
	}

	void createInstance()
	{
		// Optional
		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		// VK_API_VERSION_1_0 is VK_MAKE_VERSION(1, 0, 0)
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Compulsary
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Tell vulkan what all extensions are required
		// GLFW does that
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		// Global validation layers
		if (!enableValidationLayer) {
			createInfo.enabledLayerCount = 0;
		}
		else {
			if (!checkValidationLayerSupport())
			{
				throw std::runtime_error("Validation Layer init Error");
			}
			createInfo.enabledLayerCount = validLayersCount;
			createInfo.ppEnabledLayerNames = validLayers;
		}

		// All the availabale extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "required extensions:" << std::endl;

		for (unsigned int i = 0; i < glfwExtensionCount; i++)
			std::cout << '\t' << glfwExtensions[i] << std::endl;

		std::cout << "\nAll Extensions\n" << std::endl;

		for (const auto& extension : extensions)
			std::cout << "\t" << extension.extensionName << std::endl;

		// Add all the required extensions in createInfo
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
	}

	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		// If you terminate or error occurs close the window else poll the events on window
		// Polls events from event queue
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		vkDestroyInstance(instance, nullptr);

		//destroy window and terminate glfw
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void initWindow() {
		// Initialize GLFW
		glfwInit();

		// Hint not to create OGL window
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// Hint that window is not resizeable
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// 4th parameter is monitor	to open the window on (multi moniter setup) nullptr is default
		// 5th parameter is for ogl
		window = glfwCreateWindow(width, height, "Triangle", nullptr, nullptr);
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}