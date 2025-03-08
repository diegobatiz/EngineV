#pragma once

namespace EngineV
{
	class Renderer;
	struct QueueFamilyIndices;
	struct SwapChainSupportDetails;

	class PhysicalDevice
	{
	public:
		PhysicalDevice(const Renderer& renderer);
		void Initialize();
		void Terminate();
		QueueFamilyIndices GetQueueFamily();
		SwapChainSupportDetails GetSwapDetails();
		VkPhysicalDevice GetDevice() const { return mPhysicalDevice; }

	private:
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	private:
		const Renderer* mRenderer;
		QueueFamilyIndices* mQueueFamily = nullptr;
		SwapChainSupportDetails* mSwapchainDetails = nullptr;

		VkPhysicalDevice mPhysicalDevice;
	};
}