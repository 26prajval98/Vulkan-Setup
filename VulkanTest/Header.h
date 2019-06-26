#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include<iostream>
#include <vector>
#include <assert.h>
#include <algorithm> 
#include <fstream>
#include <array>
#include<chrono>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;

//#include "initialiser.h"
//#include "instance.h"
//#include "physicaldevice.h"
//#include "device.h"
//#include "window.h"
//#include "windowsurface.h"