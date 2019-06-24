//#include "Header.h"

#include "defines.h"
#include "device.h"
#include "physicaldevice.h"
#include "instance.h"
#include "physicaldevice.h"
#include "pipeline.h"
#include "queuefamily.h"
#include "renderpass.h"
#include "shaders.h"
#include "swapchain.h"
#include "window.h"
#include "windowsurface.h"

Window * window;
Instance * instance;
WindowSurface * windowSurface;
PhysicalDevice * physicalDevice;
Device * device;
SwapChain * swapChain;
Shaders * shaders;
RenderPass * renderPass;
Pipeline * pipeline;

int main() {
	window = new Window();
	instance = new Instance();
	windowSurface = new WindowSurface(instance, window);
	physicalDevice = PhysicalDevice::GetPhysicalDevice(instance);

	ASSERT_VALUE(physicalDevice->doesSupportWindowSurface(windowSurface), VK_TRUE, "SURFACE NOT SUPPORTED");

	device = new Device(instance, physicalDevice);
	
	VkCommandBuffer * commands = new VkCommandBuffer[3];

	device->getComputeCommand(commands, 3);

	device->freeComputeCommand(commands, 3);

	swapChain = new SwapChain(device, physicalDevice, windowSurface);

	shaders = new Shaders(device);

	renderPass = new RenderPass(device, swapChain);
	
	pipeline = new Pipeline(device, renderPass, shaders, swapChain);

	delete(pipeline);
	delete(renderPass);
	delete(shaders);
	delete(swapChain);
	delete(device);
	delete(physicalDevice);
	delete(windowSurface);
	delete(instance);
	delete(window);
	return 0;
}