#include "Header.h"

Instance * instance;
PhysicalDevice * physicalDevice;
Device * device;

int main() {
	instance = new Instance();
	physicalDevice = PhysicalDevice::GetPhysicalDevice(instance);
	device = new Device(instance, physicalDevice);
	
	VkCommandBuffer * commands = new VkCommandBuffer[3];

	device->getComputeCommand(commands, 3);

	device->freeComputeCommand(commands, 3);

	delete(device);
	delete(physicalDevice);
	delete(instance);
	return 0;
}