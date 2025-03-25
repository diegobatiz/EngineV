#pragma once

namespace EngineV
{
	class Renderer;
	class Window;
	class CommandPool;
	struct QueueFamilyIndices;
	struct SwapChainSupportDetails;

	class SwapChain
	{
	public:
		SwapChain(const Renderer& renderer, const Window& window);
		void Initialize(QueueFamilyIndices indices, SwapChainSupportDetails details);
		void CreateFramebuffers(VkRenderPass renderPass);
		void CreateDepthResources(VkFormat depthFormat, const CommandPool& commandPool);
		void Terminate();
		void Recreate();
		const VkFormat GetSwapFormat() const { return mSwapChainImageFormat; }
		VkExtent2D GetExtent() const { return mSwapChainExtent; }
		VkSwapchainKHR GetSwapchain() const { return mSwapChain; }

	private:
		void CreateSwapchain(QueueFamilyIndices indices, SwapChainSupportDetails details);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		void CreateImageViews();

	private:
		const Renderer* mRenderer = nullptr;
		const Window* mWindow = nullptr;
		const CommandPool* mCommandPool = nullptr;

		VkSwapchainKHR mSwapChain;
		std::vector<VkFramebuffer> mSwapChainFramebuffers;

		std::vector<VkImage> mSwapChainImages;
		VkFormat mSwapChainImageFormat;
		VkExtent2D mSwapChainExtent;
		std::vector<VkImageView> mSwapChainImageViews;

		VkImage mDepthImage;
		VkDeviceMemory mDepthImageMemory;
		VkImageView mDepthImageView;


		QueueFamilyIndices mIndices;
		SwapChainSupportDetails mDetails;
		VkFormat mDepthFormat; 
		VkRenderPass mRenderPass;
	};
}