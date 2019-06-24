#pragma once

#include "Header.h"
#include "swa"

class Renderpass
{
public:
	Renderpass();
	~Renderpass();

private:

};

Renderpass::Renderpass()
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
}

Renderpass::~Renderpass()
{
}