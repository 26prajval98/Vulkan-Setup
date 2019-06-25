//#include "Header.h"

#include "buffers.h"
#include "draw.h"
#include "defines.h"
#include "device.h"
#include "framebuffer.h"
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
#include "semaphores.h"

Window * window;
Instance * instance;
WindowSurface * windowSurface;
PhysicalDevice * physicalDevice;
Device * device;
SwapChain * swapChain;
Shaders * shaders;
RenderPass * renderPass;
Pipeline * pipeline;
FrameBuffer * frameBuffer;
Semaphore * semaphore;
Draw * draw;
VertexBuffer * vertexBuffer;

int main() {
	window = new Window();
	instance = new Instance();
	windowSurface = new WindowSurface(instance, window);
	physicalDevice = PhysicalDevice::GetPhysicalDevice(instance);

	ASSERT_VALUE(physicalDevice->doesSupportWindowSurface(windowSurface), VK_TRUE, "SURFACE NOT SUPPORTED");

	device = new Device(instance, physicalDevice);

	swapChain = new SwapChain(device, physicalDevice, windowSurface);

	shaders = new Shaders(device);

	renderPass = new RenderPass(device, swapChain);

	auto vertices = std::vector<Vertex>({
		{{0.0, -0.5}, {1.0, 0.0, 0.0}},
		{{0.5, 0.5}, {1.0, 0.0, 0.0}},
		{{0.5, -0.5}, {1.0, 0.0, 0.0}},
	});

	vertexBuffer = new VertexBuffer(physicalDevice, device, vertices);

	pipeline = new Pipeline(device, renderPass, shaders, swapChain);

	frameBuffer = new FrameBuffer(device, swapChain, renderPass);

	auto cmdGraphicsBufferSize = swapChain->getImageViews().size();

	auto commands = std::vector<VkCommandBuffer>(cmdGraphicsBufferSize);

	semaphore = new Semaphore(device);

	device->getGraphicsCommand(commands, U(cmdGraphicsBufferSize));

	// vkBeginCommandBuffer will always reset the command buffer

	renderPass->command(commands, frameBuffer->getFrameBuffer(), pipeline->getGraphicsPipeline(), vertexBuffer, 0, vertexBuffer->getNoVertices());

	draw = new Draw(window, device, swapChain, semaphore, commands);

	device->freeGraphicsCommand(commands, U(cmdGraphicsBufferSize));

	delete(semaphore);
	delete(frameBuffer);
	delete(pipeline);
	delete(renderPass);
	delete(vertexBuffer);
	delete(shaders);
	delete(swapChain);
	delete(device);
	delete(physicalDevice);
	delete(windowSurface);
	delete(instance);
	delete(window);
	return 0;
}