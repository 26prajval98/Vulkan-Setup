#include "Header.h"

Instance * instance;
PhysicalDevice * physicalDevice;
Device * device;
Window * window;
WindowSurface * windowSurface;

int main() {
	window = new Window();
	instance = new Instance();
	windowSurface = new WindowSurface(instance, window);
	physicalDevice = PhysicalDevice::GetPhysicalDevice(instance);
	device = new Device(instance, physicalDevice);
	
	VkCommandBuffer * commands = new VkCommandBuffer[3];

	device->getComputeCommand(commands, 3);

	device->freeComputeCommand(commands, 3);

	delete(device);
	delete(physicalDevice);
	delete(windowSurface);
	delete(instance);
	delete(window);
	return 0;
}