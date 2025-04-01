#include "Precompiled.h"
#include "SwapChain.h"

#include "Renderer.h"
#include "Window.h"
#include "ImageCreation.h"
#include "CommandPool.h"

using namespace EngineV;

SwapChain::SwapChain(const Renderer& renderer, const Window& window)
{
	mRenderer = &renderer;
	mWindow = &window;
}

void SwapChain::Initialize(QueueFamilyIndices indices, SwapChainSupportDetails details)
{
	mIndices = indices;
	mDetails = details;
	CreateSwapchain(mIndices, mDetails);
	CreateImageViews();
}

void SwapChain::CreateFramebuffers(VkRenderPass renderPass)
{
	mSwapChainFramebuffers.resize(mSwapChainImageViews.size());
	mRenderPass = renderPass;

	for (size_t i = 0; i < mSwapChainImageViews.size(); ++i)
	{
		std::array<VkImageView, 2> attachments =
		{
			mSwapChainImageViews[i],
			mDepthImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = mSwapChainExtent.width;
		framebufferInfo.height = mSwapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(mRenderer->GetDevice(), &framebufferInfo, nullptr, &mSwapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create frame buffer");
		}
	}
}

void SwapChain::CreateDepthResources(VkFormat depthFormat, const CommandPool& commandPool)
{
	mDepthFormat = depthFormat;
	mCommandPool = &commandPool;
	CreateImage(*mRenderer, mSwapChainExtent.width, mSwapChainExtent.height, mDepthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mDepthImage, mDepthImageMemory);
	mDepthImageView = CreateImageView(mRenderer->GetDevice(), mDepthImage, mDepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

	TransitionImageLayout(commandPool, mDepthImage, mDepthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void SwapChain::Terminate()
{
	VkDevice device = mRenderer->GetDevice();
	for (size_t i = 0; i < mSwapChainFramebuffers.size(); ++i)
	{
		vkDestroyFramebuffer(device, mSwapChainFramebuffers[i], nullptr);
	}

	vkDestroyImageView(device, mDepthImageView, nullptr);
	vkDestroyImage(device, mDepthImage, nullptr);
	vkFreeMemory(device, mDepthImageMemory, nullptr);

	for (size_t i = 0; i < mSwapChainImageViews.size(); ++i)
	{
		vkDestroyImageView(device, mSwapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(device, mSwapChain, nullptr);
}

void SwapChain::Recreate(SwapChainSupportDetails details)
{
	int width = 0, height = 0;

	width = mWindow->GetWindowWidth();
	height = mWindow->GetWindowHeight();
	while (width == 0 || height == 0)
	{
		width = mWindow->GetWindowWidth();
		height = mWindow->GetWindowHeight();
		glfwWaitEvents();
	}


	vkDeviceWaitIdle(mRenderer->GetDevice());

	Terminate();
	mDetails = details;
	CreateSwapchain(mIndices, mDetails);
	CreateImageViews();
	CreateDepthResources(mDepthFormat, *mCommandPool);
	CreateFramebuffers(mRenderPass);
}

void EngineV::SwapChain::BindViewport(VkCommandBuffer commandBuffer)
{
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(mSwapChainExtent.width);
	viewport.height = static_cast<float>(mSwapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
}

void EngineV::SwapChain::BindScissor(VkCommandBuffer commandBuffer)
{
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = mSwapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void SwapChain::CreateSwapchain(QueueFamilyIndices indices, SwapChainSupportDetails details)
{
	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(details.presentModes);
	VkExtent2D extent = ChooseSwapExtent(details.capabilities);

	uint32_t imageCount = MAX_FRAMES_IN_FLIGHT;
	if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
	{
		imageCount = details.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = mRenderer->GetSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	//This image usage renders directly to the swap chain. 
	//If we want to do post processing, use something else like VK_IMAGE_USAGE_TRANSFER_DST_BIT

	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	createInfo.preTransform = details.capabilities.currentTransform;
	//Apply a transformation to the images in swap chain
	//ex. 90 degrees rotation or horizontal flip

	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	//Don't care about pixels that are obscured by other windows if clipped is true

	createInfo.oldSwapchain = VK_NULL_HANDLE; //CHANGE THIS LATER

	VkDevice device = mRenderer->GetDevice();

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &mSwapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create swap chain");
	}

	vkGetSwapchainImagesKHR(device, mSwapChain, &imageCount, nullptr);
	mSwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, mSwapChain, &imageCount, mSwapChainImages.data());

	mSwapChainImageFormat = surfaceFormat.format;
	mSwapChainExtent = extent;
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	//Change this if need to use B16G16R16A16 in the future (HDR color space)
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	//If energy is a concern (phones for example) use FIFO (VSync)
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width = mWindow->GetWindowWidth();
		int height = mWindow->GetWindowHeight();

		VkExtent2D actualExtent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void SwapChain::CreateImageViews()
{
	mSwapChainImageViews.resize(mSwapChainImages.size());
	VkDevice device = mRenderer->GetDevice();

	for (size_t i = 0; i < mSwapChainImages.size(); ++i)
	{
		mSwapChainImageViews[i] = CreateImageView(device, mSwapChainImages[i], mSwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}
