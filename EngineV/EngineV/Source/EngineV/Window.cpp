#include "../Precompiled.h"
#include "Window.h"

EngineV::Window::Window()
{
}

EngineV::Window::~Window()
{
}

void EngineV::Window::InitWindow(int width, int height)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	mWindow = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(mWindow, this);
	//glfwSetFramebufferSizeCallback(mWindow, framebufferResizeCallback);
}
