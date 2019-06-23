//#include "Header.h"

#include "defines.h"
#include "device.h"
#include "physicaldevice.h"
#include "instance.h"
#include "physicaldevice.h"
#include "queuefamily.h"
#include "swapchain.h"
#include "window.h"
#include "windowsurface.h"

Instance * instance;
PhysicalDevice * physicalDevice;
Device * device;
SwapChain * swapChain;
Window * window;
WindowSurface * windowSurface;

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

	delete(swapChain);
	delete(device);
	delete(physicalDevice);
	delete(windowSurface);
	delete(instance);
	delete(window);
	return 0;
}