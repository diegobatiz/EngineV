#pragma once

namespace EngineV
{
	class Renderer;
	class UniformBuffer;

	class DescriptorPool
	{
	public:
		DescriptorPool(const Renderer& renderer);
		void Initalize(uint32_t count);
		void InitializeDescriptorSets(VkDescriptorSetLayout layout, const std::vector<UniformBuffer>& buffers, VkImageView imageView, VkSampler sampler);
		void Terminate();
		void Bind(VkCommandBuffer commandBuffer, int frame, VkPipelineLayout layout);

	private:
		const Renderer* mRenderer = nullptr;
		VkDescriptorPool mDescriptorPool;
		std::vector<VkDescriptorSet> mDescriptorSets;
		uint32_t mFrameCount;
	};
}