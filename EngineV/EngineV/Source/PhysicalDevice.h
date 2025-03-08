#pragma once

namespace EngineV
{
	class Renderer;
	struct QueueFamilyIndices;
	struct SwapChainSupportDetails;

	class PhysicalDevice
	{
	public:
		PhysicalDevice(Renderer& renderer);
		void Initialize();
		void Terminate();
		QueueFamilyIndices GetQueueFamily();
		SwapChainSupportDetails GetSwapDetails();
		VkPhysicalDevice* GetDevice() { return &mPhysicalDevice; }

	private:
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	private:
		Renderer* mRenderer;
		VkPhysicalDevice mPhysicalDevice;
		QueueFamilyIndices* mQueueFamily;
		SwapChainSupportDetails* mSwapchainDetails;
	};
}