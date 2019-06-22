#pragma once

#include "Header.h"

class WindowSurface
{
public:
	WindowSurface(Instance * instance, Window * window);
	~WindowSurface();

private:
	VkSurfaceKHR m_surface;
	Instance * m_instance;
	Window * m_window;
};

WindowSurface::WindowSurface(Instance * instance, Window * window) : m_instance(instance), m_window(window)
{
	ASSERT(glfwCreateWindowSurface(instance->getInstance(), window->getWindow(), nullptr, &m_surface), "failed to create window surface!");
}

WindowSurface::~WindowSurface()
{
	vkDestroySurfaceKHR(m_instance->getInstance(), m_surface, nullptr);
}