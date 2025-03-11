#pragma once
#include "VertexTypes.h"

namespace EngineV
{
	class Renderer;

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(const Renderer& renderer);
		void Initialize(const Vertex& vertexType, VkExtent2D swapChainExtent, VkDescriptorSetLayout layout, VkRenderPass renderPass);
		void Terminate();

	private:
		VkShaderModule CreateShaderModule(const std::vector<char>& code);

	private:
		const Renderer* mRenderer = nullptr;
		VkPipelineLayout mLayout;
		VkPipeline mGraphicsPipeline;
	};
}