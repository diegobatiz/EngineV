#include "Precompiled.h"
#include "Renderer.h"

#include "Window.h"
#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "DescriptorSetLayout.h"
#include "GraphicsPipeline.h"
#include "CommandPool.h"
#include "Texture.h"
#include "DescriptorPool.h"

namespace
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::cerr << "Validation Layer: " << pCallbackData->pMessage << "\n";

		return VK_FALSE;
	}

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, debugMessenger, pAllocator);
		}
	}
}

EngineV::Renderer::Renderer(const char* appName, const Window& window)
{
	mAppName = appName;
	mWindow = &window;
	mPhysicalDevice = new PhysicalDevice(*this);
	mSwapChain = new SwapChain(*this, *mWindow);
	mRenderPass = new RenderPass(*this);
	mLayout = new DescriptorSetLayout(*this);
	mGraphicsPipeline = new GraphicsPipeline(*this);
	mCommandPool = new CommandPool(*this);
	mLandscapeTexture = new Texture(*this);
	mVertexBuffer = new TypedBuffer<VertexPCT>();
	mIndexBuffer = new TypedBuffer<uint16_t>();

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		mUniformBuffers.push_back(UniformBuffer());
	}

	mDescriptorPool = new DescriptorPool(*this);
}

EngineV::Renderer::~Renderer()
{
	delete mDescriptorPool;
	mDescriptorPool = nullptr;

	delete mVertexBuffer;
	mVertexBuffer = nullptr;
	delete mIndexBuffer;
	mIndexBuffer = nullptr;

	delete mLandscapeTexture;
	mLandscapeTexture = nullptr;

	delete mCommandPool;
	mCommandPool = nullptr;

	delete mGraphicsPipeline;
	mGraphicsPipeline = nullptr;

	delete mLayout;
	mLayout = nullptr;

	delete mRenderPass;
	mRenderPass = nullptr;

	delete mSwapChain;
	mSwapChain = nullptr;

	delete mPhysicalDevice;
	mPhysicalDevice = nullptr;
	
	mWindow = nullptr;
}

void EngineV::Renderer::Initialize()
{
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	mPhysicalDevice->Initialize();
	CreateLogicalDevice();
	mSwapChain->Initialize(mPhysicalDevice->GetQueueFamily(), mPhysicalDevice->GetSwapDetails());
	mRenderPass->Initalize(mSwapChain->GetSwapFormat());
	mLayout->Initalize(1, 1);
	VertexPCT vertex = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0} };
	mGraphicsPipeline->Initialize(vertex, mSwapChain->GetExtent(), mLayout->GetLayout(), mRenderPass->GetRenderPass());
	mCommandPool->Initalize(mPhysicalDevice->GetQueueFamily());
	mSwapChain->CreateDepthResources(mPhysicalDevice->GetDepthFormat(), *mCommandPool);
	mSwapChain->CreateFramebuffers(mRenderPass->GetRenderPass());
	mLandscapeTexture->Initalize(*mCommandPool);
	mVertexBuffer->Initialize(*this, *mCommandPool, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices);
	mIndexBuffer->Initialize(*this, *mCommandPool, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indices);
	for (auto buffer : mUniformBuffers)
	{
		buffer.Initialize<WorldView>(*this, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	}
	mDescriptorPool->Initalize(static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT));
	mDescriptorPool->InitializeDescriptorSets(mLayout->GetLayout(), mUniformBuffers, mLandscapeTexture->GetImageView(), mLandscapeTexture->GetSampler());
	mCommandPool->CreateCommandBuffers();
}

void EngineV::Renderer::Terminate()
{
	for (auto buffer : mUniformBuffers)
	{
		buffer.Terminate();
	}
	mIndexBuffer->Terminate();
	mVertexBuffer->Terminate();
	mSwapChain->Terminate();
	mLandscapeTexture->Terminate();
	mLayout->Terminate();
	mRenderPass->Terminate();
	mPhysicalDevice->Terminate();
	mCommandPool->Terminate();

	vkDestroyDevice(mDevice, nullptr);
	if (gEnableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	vkDestroyInstance(mInstance, nullptr);
}

VkPhysicalDevice EngineV::Renderer::GetPhysicalDevice() const
{
	return mPhysicalDevice->GetDevice();
}

void EngineV::Renderer::CreateInstance()
{
	if (gEnableValidationLayers && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = mAppName;
	appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.pEngineName = "Engine V";
	appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (gEnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(gValidationLayers.size());
		createInfo.ppEnabledLayerNames = gValidationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance");
	}
}

bool EngineV::Renderer::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : gValidationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

std::vector<const char*> EngineV::Renderer::GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (gEnableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void EngineV::Renderer::SetupDebugMessenger()
{
	if (!gEnableValidationLayers)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger");
	}
}

void EngineV::Renderer::CreateSurface()
{
	GLFWwindow* window = mWindow->GetWindowPointer();
	if (glfwCreateWindowSurface(mInstance, window, nullptr, &mSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface");
	}
}

void EngineV::Renderer::CreateLogicalDevice()
{
	QueueFamilyIndices indices = mPhysicalDevice->GetQueueFamily();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(gDeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = gDeviceExtensions.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	if (gEnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(gValidationLayers.size());
		createInfo.ppEnabledLayerNames = gValidationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(mPhysicalDevice->GetDevice(), &createInfo, nullptr, &mDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(mDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
	vkGetDeviceQueue(mDevice, indices.presentFamily.value(), 0, &mPresentQueue);
}

void EngineV::Renderer::CreateSyncObjects()
{
	mImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	mRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	mInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		if (vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mImageAvailableSemaphores[i]) != VK_SUCCESS
			|| vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mRenderFinishedSemaphores[i]) != VK_SUCCESS
			|| vkCreateFence(mDevice, &fenceInfo, nullptr, &mInFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create semaphores");
		}
	}
}
