#pragma once
#include "VertexTypes.h"

namespace EngineV
{
	class Window;
	class PhysicalDevice;
	class SwapChain;
	class RenderPass;
	class DescriptorSetLayout;
	class GraphicsPipeline;
	class CommandPool;
	class Texture;
	class Buffer;

#ifdef NDEBUG
	const bool gEnableValidationLayers = false;
#else
	const bool gEnableValidationLayers = true;
#endif

	const std::vector<const char*> gValidationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> gDeviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

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

	const std::vector<VertexPCT> vertices =
	{
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{-0.5f, -0.5f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f,  0.5f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices =
	{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	class Renderer
	{
	public:
		Renderer(const char* appName, const Window& window);
		~Renderer();

		void Initialize();
		void Terminate();

		VkInstance GetInstance() const { return mInstance; }
		VkSurfaceKHR GetSurface() const { return mSurface; }
		VkDevice GetDevice() const { return mDevice; }
		VkQueue GetGraphicsQueue() const { return mGraphicsQueue; }
		VkPhysicalDevice GetPhysicalDevice() const;

	private:
		void CreateInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();

		void SetupDebugMessenger();
		void CreateSurface();
		void CreateLogicalDevice();

	private:
		const char* mAppName;
		const Window* mWindow;
		PhysicalDevice* mPhysicalDevice = nullptr;
		SwapChain* mSwapChain = nullptr;
		RenderPass* mRenderPass = nullptr;
		DescriptorSetLayout* mLayout = nullptr;
		GraphicsPipeline* mGraphicsPipeline = nullptr;
		CommandPool* mCommandPool = nullptr;
		Texture* mLandscapeTexture = nullptr;
		Buffer* mVertexBuffer = nullptr;
		Buffer* mIndexBuffer = nullptr;

		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		VkSurfaceKHR mSurface;
		VkDevice mDevice;

		VkQueue mGraphicsQueue;
		VkQueue mPresentQueue;
	};
}