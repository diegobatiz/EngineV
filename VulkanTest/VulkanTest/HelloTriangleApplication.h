#pragma once
#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
#include <set>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> gValidationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool gEnableValidationLayers = true;
#endif

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class HelloTriangleApplication
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	//Window Initialization
	void initWindow();

	//Vulkan Initialization
	void initVulkan();
	
	void createInstance();
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	
	void setupDebugMessenger();

	void createSurface();

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void createLogicalDevice();
	
	//Main Loop
	void mainLoop();

	void cleanup();

private:
	GLFWwindow* mWindow = nullptr;

	VkInstance mInstance;
	
	VkDebugUtilsMessengerEXT mDebugMessenger;
	
	VkSurfaceKHR mSurface;

	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceFeatures mDeviceFeatures{};
	
	VkDevice mDevice;
	
	VkQueue mGraphicsQueue;
	VkQueue mPresentQueue;
};