#pragma once

namespace EngineV
{
	class Renderer;

	class RenderPass
	{
	public:
		RenderPass(const Renderer& renderer);
		void Initalize(VkFormat swapChainFormat);
		void Terminate();

	private:
		const Renderer* mRenderer = nullptr;
		VkRenderPass mRenderPass;
	};
}
