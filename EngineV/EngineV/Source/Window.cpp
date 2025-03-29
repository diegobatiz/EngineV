#include "Precompiled.h"
#include "Window.h"
#include "Application.h"

namespace
{
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<EngineV::Application*>(glfwGetWindowUserPointer(window));
		app->ResizeWindow();
	}
}

EngineV::Window::Window()
{
}

EngineV::Window::~Window()
{
}

void EngineV::Window::InitWindow(int width, int height, Application* app)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	mWindow = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(mWindow, app);
	glfwSetFramebufferSizeCallback(mWindow, FramebufferResizeCallback);
}

void EngineV::Window::Terminate()
{
	glfwDestroyWindow(mWindow);

	glfwTerminate();
}

int EngineV::Window::GetWindowHeight() const
{
	int width;
	glfwGetFramebufferSize(mWindow, &width, nullptr);
	return width;
}

int EngineV::Window::GetWindowWidth() const
{
	int height;
	glfwGetFramebufferSize(mWindow, nullptr, &height);
	return height;
}

bool EngineV::Window::GetWindowClosed()
{
	return glfwWindowShouldClose(mWindow);
}

void EngineV::Window::PollEvents()
{
	glfwPollEvents();
	std::this_thread::sleep_for(std::chrono::milliseconds(33));
}
