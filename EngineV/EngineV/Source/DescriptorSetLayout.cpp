#include "Precompiled.h"
#include "DescriptorSetLayout.h"

#include "Renderer.h"

using namespace EngineV;

DescriptorSetLayout::DescriptorSetLayout(const Renderer& renderer)
{
	mRenderer = &renderer;
}

void DescriptorSetLayout::Initalize(int bufferCount, int samplerCount)
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = bufferCount;
	//increase if more uniform buffers
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = samplerCount;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkDevice device = mRenderer->GetDevice();
	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &mLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor set layout");
	}
}

void DescriptorSetLayout::Terminate()
{
	VkDevice device = mRenderer->GetDevice();
	vkDestroyDescriptorSetLayout(device, mLayout, nullptr);
}
