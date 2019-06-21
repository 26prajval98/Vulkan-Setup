#include "Header.h"

Instance * instance;
PhysicalDevice * physicalDevice;
Device * device;

int main() {
	instance = new Instance();
	physicalDevice = PhysicalDevice::GetPhysicalDevice(instance);
	device = new Device(instance, physicalDevice);

	delete(device);
	delete(physicalDevice);
	delete(instance);
	return 0;
}