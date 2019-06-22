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

	WindowSurface(Instance * instance, Window * window);

	~WindowSurface();

private:
	VkSurfaceKHR m_windowSurface;
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