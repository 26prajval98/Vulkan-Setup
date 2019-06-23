#pragma once

#include "Header.h"
#include "defines.h"
#include "instance.h"
#include "window.h"

class WindowSurface
{
public:
	VkSurfaceKHR getWindowSurface() {
		return m_windowSurface;
	};

	VkSurfaceCapabilitiesKHR getSurfaceWindowCapabilities(VkPhysicalDevice physicalDevice) {
		ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_windowSurface, &m_windowSurfaceCapabilities), "SURFACE CAPABILITIES NOT FOUND");
		return m_windowSurfaceCapabilities;
	}

	VkPresentModeKHR * getSurfacePresentModes(VkPhysicalDevice physicalDevice) {
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_windowSurface, &countWindowSurfacePresentModes, nullptr);
		m_pWindowSurfacePresentModes = new VkPresentModeKHR[countWindowSurfacePresentModes];
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_windowSurface, &countWindowSurfacePresentModes, m_pWindowSurfacePresentModes);
		return m_pWindowSurfacePresentModes;
	}

	WindowSurface(Instance * instance, Window * window);

	~WindowSurface();

private:
	VkSurfaceKHR m_windowSurface;
	VkSurfaceCapabilitiesKHR m_windowSurfaceCapabilities;
	uint32_t countWindowSurfacePresentModes;
	VkPresentModeKHR * m_pWindowSurfacePresentModes;

	Instance * m_instance;
	Window * m_window;
};

WindowSurface::WindowSurface(Instance * instance, Window * window) : m_instance(instance), m_window(window)
{
	ASSERT(glfwCreateWindowSurface(instance->getInstance(), window->getWindow(), nullptr, &m_windowSurface), "failed to create window surface!");
}

WindowSurface::~WindowSurface()
{
	vkDestroySurfaceKHR(m_instance->getInstance(), m_windowSurface, nullptr);
}