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

	private:
		const Renderer* mRenderer = nullptr;
		VkRenderPass mRenderPass;
	};
}
