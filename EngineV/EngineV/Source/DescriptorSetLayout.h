#pragma once

namespace EngineV
{
	class Renderer;

	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout(const Renderer& renderer);
		void Initalize(int bufferCount, int samplerCount);
		void Terminate();

	private:
		const Renderer* mRenderer = nullptr;
		VkDescriptorSetLayout mLayout;
	};
}
