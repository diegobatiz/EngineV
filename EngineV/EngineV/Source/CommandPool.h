#pragma once

namespace EngineV
{
	class Renderer;
	struct QueueFamilyIndices;

	class CommandPool
	{
	public:
		CommandPool(const Renderer& renderer);
		void Initalize(QueueFamilyIndices indices);
		void CreateCommandBuffers();
		void Terminate();
		VkCommandPool GetCommandPool() const { return mCommandPool; }
		VkCommandBuffer BeginSingleTimeCommands() const;
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;

	private:
		const Renderer* mRenderer = nullptr;
		VkCommandPool mCommandPool;
		std::vector<VkCommandBuffer> mCommandBuffers;
	};
}
