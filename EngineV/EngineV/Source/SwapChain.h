#pragma once

namespace EngineV
{
	class Renderer;
	class Window;
	struct QueueFamilyIndices;
	struct SwapChainSupportDetails;

	class SwapChain
	{
	public:
		SwapChain(const Renderer& renderer, const Window& window);
		void Initialize(QueueFamilyIndices indices, SwapChainSupportDetails details);
		void CreateFramebuffers(VkRenderPass renderPass);
		void Terminate();
		const VkFormat GetSwapFormat() const { return mSwapChainImageFormat; }

	private:
		void CreateSwapchain(QueueFamilyIndices indices, SwapChainSupportDetails details);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		void CreateImageViews();

	private:
		const Renderer* mRenderer = nullptr;
		const Window* mWindow = nullptr;

		VkSwapchainKHR mSwapChain;
		std::vector<VkFramebuffer> mSwapChainFramebuffers;

		std::vector<VkImage> mSwapChainImages;
		VkFormat mSwapChainImageFormat;
		VkExtent2D mSwapChainExtent;
		std::vector<VkImageView> mSwapChainImageViews;
	};
}