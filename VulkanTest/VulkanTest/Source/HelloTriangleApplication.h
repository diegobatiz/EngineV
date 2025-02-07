#pragma once
#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> gValidationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> gDeviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
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

	bool mFrameBufferResized = false;

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
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	void createLogicalDevice();

	void createSwapChain();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void createImageViews();

	void createRenderPass();

	void createGraphicsPipeline();
	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createFrameBuffers();

	void createCommandPool();
	
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void createSyncObjects();

	//Main Loop
	void mainLoop();

	void drawFrame();

	void recreateSwapChain();
	void cleanupSwapChain();

	void cleanup();

private:
	GLFWwindow* mWindow = nullptr;

	VkInstance mInstance;
	
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceFeatures mDeviceFeatures{};
	
	VkDebugUtilsMessengerEXT mDebugMessenger;
	VkSurfaceKHR mSurface;
	VkDevice mDevice;
	VkSwapchainKHR mSwapChain;
	VkRenderPass mRenderPass;
	VkPipeline mGraphicsPipeline;
	std::vector<VkFramebuffer> mSwapChainFramebuffers;
	VkCommandPool mCommandPool;
	std::vector<VkCommandBuffer> mCommandBuffers;
	
	VkQueue mGraphicsQueue;
	VkQueue mPresentQueue;

	std::vector<VkImage> mSwapChainImages;
	VkFormat mSwapChainImageFormat;
	VkExtent2D mSwapChainExtent;
	std::vector<VkImageView> mSwapChainImageViews;

	VkPipelineLayout mPipelineLayout;

	std::vector<VkSemaphore> mImageAvailableSemaphores;
	std::vector<VkSemaphore> mRenderFinishedSemaphores;
	std::vector<VkFence> mInFlightFences;
		
	uint32_t mCurrentFrame = 0;
};