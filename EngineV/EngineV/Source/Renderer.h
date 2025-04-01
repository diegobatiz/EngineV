#pragma once
#include "VertexTypes.h"
#include "Buffer.h"

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
	class DescriptorPool;

#ifdef NDEBUG
	const bool gEnableValidationLayers = false;
#else
	const bool gEnableValidationLayers = true;
#endif

#define MAX_FRAMES_IN_FLIGHT 1

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

		bool isComplete() const
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
		{ {-0.5, -0.5, 0.0}, {1.0, 1.0, 1.0}, {1.0, 0.0} },
		{ { 0.5, -0.5, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0} },
		{ { 0.5,  0.5, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0} },
		{ {-0.5,  0.5, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0} }
	};

	const std::vector<uint16_t> indices =
	{
		0, 1, 2, 2, 3, 0
	};

	struct WorldView
	{
		WorldView() = default;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	class Renderer
	{
	public:
		Renderer(const char* appName, const Window& window);
		~Renderer();

		void Initialize();
		void Terminate();
		void WindowResized(bool resized) { mFrameBufferResized = resized; }
		void DrawFrame();

		VkInstance GetInstance() const { return mInstance; }
		VkSurfaceKHR GetSurface() const { return mSurface; }
		VkDevice GetDevice() const { return mDevice; }
		VkQueue GetGraphicsQueue() const { return mGraphicsQueue; }
		VkPhysicalDevice GetPhysicalDevice() const;
		float GetWidth() const;
		float GetHeight() const;

	private:
		void CreateInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();

		void SetupDebugMessenger();
		void CreateSurface();
		void CreateLogicalDevice();
		void CreateSyncObjects();

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
		TypedBuffer mVertexBuffer;
		TypedBuffer mIndexBuffer;
		std::vector<UniformBuffer> mUniformBuffers;
		DescriptorPool* mDescriptorPool = nullptr;

		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		VkSurfaceKHR mSurface;
		VkDevice mDevice;

		VkQueue mGraphicsQueue;
		VkQueue mPresentQueue;

		std::vector<VkSemaphore> mImageAvailableSemaphores;
		std::vector<VkSemaphore> mRenderFinishedSemaphores;
		std::vector<VkFence> mInFlightFences;

		uint32_t mCurrentFrame = 0;
		bool mFrameBufferResized = false;
	};
}