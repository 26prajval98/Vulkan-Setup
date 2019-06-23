#pragma once

#include "Header.h"

class Window
{
public:
	Window();
	~Window();
	GLFWwindow* getWindow() {
		return window;
	}

private:
	GLFWwindow* window;
};

Window::Window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}