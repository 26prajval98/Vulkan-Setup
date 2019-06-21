#pragma once

#include "Header.h"

struct QueueFamilyIndices {
	uint32_t graphicIndices = UINT32_MAX;
	uint32_t computeIndices = UINT32_MAX;
	bool isComplete() {
		return (graphicIndices < UINT32_MAX && computeIndices < UINT32_MAX);
	}
};

