#pragma once

namespace EngineV
{
	class Renderer;

	class RenderPass
	{
	public:
		RenderPass(const Renderer& renderer);
		void Initalize(VkFormat swapChainFormat, VkFormat depthFormat = VK_FORMAT_UNDEFINED);
		void Terminate();
		VkRenderPass GetRenderPass() const { return mRenderPass; }

		void Begin(VkFramebuffer framebuffer, VkExtent2D swapExtent, VkCommandBuffer commandBuffer, VkClearValue color);
		void End(VkCommandBuffer commandBuffer);

	private:
		const Renderer* mRenderer = nullptr;
		VkRenderPass mRenderPass;
	};
}
