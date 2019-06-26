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
IndexBuffer * indexBuffer;

const std::vector<Vertex> vertices = {
	{{0.0f, -0.5f}, {1.0f, 1.0f, .0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
};

const std::vector<uint32_t> indices = { 0, 1, 2 };

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

	vertexBuffer = new VertexBuffer(physicalDevice, device, vertices);

	indexBuffer = new IndexBuffer(physicalDevice, device, indices);

	pipeline = new Pipeline(device, renderPass, shaders, swapChain);

	frameBuffer = new FrameBuffer(device, swapChain, renderPass);

	auto cmdGraphicsBufferSize = swapChain->getImageViews().size();

	auto commands = std::vector<VkCommandBuffer>(cmdGraphicsBufferSize);

	semaphore = new Semaphore(device);

	device->getGraphicsCommand(commands, U(cmdGraphicsBufferSize));

	// vkBeginCommandBuffer will always reset the command buffer

	CommandDetails * commandDetails = (CommandDetails *)malloc(sizeof(CommandDetails));
	commandDetails->pVertexBuffer = (VkBuffer *)malloc(sizeof(VkBuffer));
	commandDetails->commandBuffer = commands;
	commandDetails->frameBuffer = frameBuffer->getFrameBuffer();
	commandDetails->graphicsPipeline = pipeline->getGraphicsPipeline();
	commandDetails->offset = 0;
	commandDetails->pVertexBuffer = vertexBuffer->pGetVertexBuffer();
	commandDetails->indexBuffer = indexBuffer->getIndexBuffer();
	commandDetails->verticesCount = vertexBuffer->getNoVertices();
	commandDetails->indicesCount = indexBuffer->getNoIndices();

	renderPass->command(commandDetails);

	draw = new Draw(window, device, swapChain, semaphore, commands);

	device->freeGraphicsCommand(commands, U(cmdGraphicsBufferSize));

	delete(semaphore);
	delete(frameBuffer);
	delete(pipeline);
	delete(renderPass);
	delete(indexBuffer);
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