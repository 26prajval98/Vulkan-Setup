#pragma once

#include "Header.h"

class Buffer
{
public:
	Buffer(Device * device);
	~Buffer();

private:
	Device * m_device;
};

Buffer::Buffer(Device * device) : m_device(device)
{
}

Buffer::~Buffer()
{
}