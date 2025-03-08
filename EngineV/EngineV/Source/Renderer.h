#pragma once

namespace EngineV
{
	class Window;
	class PhysicalDevice;
	class SwapChain;

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

		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		VkSurfaceKHR mSurface;
		VkDevice mDevice;

		VkQueue mGraphicsQueue;
		VkQueue mPresentQueue;
	};
}