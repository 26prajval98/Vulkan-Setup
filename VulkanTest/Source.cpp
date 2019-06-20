#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <optional>
#include <set>

#define DEBUG

const int width = 800;
const int height = 600;

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

// All of the useful standard validation is bundled into a layer included in the SDK that is known as VK_LAYER_KHRONOS_validation
std::vector<const char *>validLayers = { "VK_LAYER_KHRONOS_validation" };

const unsigned int validLayersCount = static_cast<unsigned int> (validLayers.size());

// Enable validation only in debug mode
#ifdef DEBUG
const bool enableValidationLayer = true;
#else
const bool enableValidationLayer = false;
#endif // DEBUG


// Extension function therefore add it manually
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}


void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}


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
	VkDebugUtilsMessengerEXT debugMessenger;
	// Initialize to no devices
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	// Can have multiple logical devices but we are choosing only 1
	VkDevice device;
	// to get any queue vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	VkQueue graphicsQueue;
	// Present queue vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	VkQueue presentQueue;
	// Creation of a surface
	VkSurfaceKHR surface;


	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}


	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		//createInfo.pUserData = nullptr; // Optional
	}


	void setupDebugMessenger() {
		if (!enableValidationLayer) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}
	}


	// VK_EXT_DEBUG_UTILS_EXTENSION_NAME is to enable validation layer to output to the window (relay debug messages)
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

		for (unsigned int i = 0; i < validLayersCount; i++)
		{
			bool layerFound = false;
			for (auto& availableLayer : availableLayers) {
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


	void createInstance() {

		if (enableValidationLayer && !checkValidationLayerSupport())
		{
			throw std::runtime_error("Validation Layer init Error");
		}

		// Optional
		VkApplicationInfo appInfo = {};
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
		/*
			//uint32_t glfwExtensionCount = 0;
			//const char** glfwExtensions;
			//glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			// Add all the required extensions in createInfo
			//createInfo.enabledExtensionCount = glfwExtensionCount;
			//createInfo.ppEnabledExtensionNames = glfwExtensions;

			// All the availabale extensions
			//uint32_t extensionCount = 0;
			//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			//std::vector<VkExtensionProperties> extensions(extensionCount);
			//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

			//std::cout << "required extensions:" << std::endl;

			//for (unsigned int i = 0; i < glfwExtensionCount; i++)
			//	std::cout << '\t' << glfwExtensions[i] << std::endl;

			//std::cout << "\nAll Extensions\n" << std::endl;

			//for (const auto& extension : extensions)
			//	std::cout << "\t" << extension.extensionName << std::endl
		*/
		auto extensions = getRequiredExtensions();

		createInfo.enabledExtensionCount = (unsigned int)extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Global validation layers
		// pNext required for debug instance created or not
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};

		if (!enableValidationLayer) {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}
		else {
			createInfo.enabledLayerCount = validLayersCount;
			createInfo.ppEnabledLayerNames = &validLayers[0];
			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
	}


	int rateDeviceSuitability(VkPhysicalDevice device) {

		int score = 0;
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		// Discrete GPUs have a significant performance advantage
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		// Maximum possible size of textures affects graphics quality
		score += deviceProperties.limits.maxImageDimension2D;

		// Application can't function without geometry shaders
		if (!deviceFeatures.geometryShader) {
			return 0;
		}

		return score;
	}


	// Pick up the best device available
	void pickPhysicalDevice() {

		// Get number of devices and put it in a vectorq		
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		// Use an ordered map to automatically sort candidates by increasing score
		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : devices) {
			QueueFamilyIndices indices = findQueueFamilies(device);
			if (indices.isComplete()) {
				int score = rateDeviceSuitability(device);
				candidates.insert(std::make_pair(score, device));
			}
		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0) {
			physicalDevice = candidates.rbegin()->second;
		}
		else {
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}


	// Select atleast 1 queue family with VK_QUEUE_GRAPHICS_BIT and add all such devices to your mapVK_QUEUE_GRAPHICS_BIT
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		VkBool32 presentSupport = false;

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			VkBool32 presentSupport = false;
			if (queueFamily.queueCount && vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport)) {
				indices.presentFamily = i;
			}
			if (indices.isComplete()) {
				break;
			}
			i++;
		}

		return indices;
	}



	void createLogicalDevice() {

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		float queuePriority = 1.0f;
		for (auto& queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			// Priority influeneces the scheduling of command buffer execution
			queueCreateInfo.pQueuePriorities = &queuePriority;
		}

		// Specify device features
		VkPhysicalDeviceFeatures deviceFeatures = {};

		// Creating the logical device i.e combine both the above structures	
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfos[0];
		createInfo.queueCreateInfoCount = static_cast<unsigned int>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		// No need for device specific but done so for compatability with old versions of Vulkan
		createInfo.enabledExtensionCount = 0;
		if (enableValidationLayer) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validLayers.size());
			createInfo.ppEnabledLayerNames = validLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		// Create logical device
		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}
		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	}


	// GLFW support. Straight forward
	void createSurface() {
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}


	void initVulkan() {
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
	}


	void mainLoop() {
		// If you terminate or error occurs close the window else poll the events on window
		// Polls events from event queue
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}


	void cleanup() {

		vkDestroyDevice(device, nullptr);

		vkDestroySurfaceKHR(instance, surface, nullptr);

		if (enableValidationLayer) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}

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