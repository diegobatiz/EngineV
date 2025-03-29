#include "Precompiled.h"
#include "Buffer.h"

#include "Renderer.h"
#include "BufferCreation.h"

using namespace EngineV;

void Buffer::Initalize(const Renderer& renderer, const CommandPool& commandPool, VkBufferUsageFlags usage, const void* vectorData, VkDeviceSize size)
{
	mUsage = usage;
	mRenderer = &renderer;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(*mRenderer, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	VkDevice device = mRenderer->GetDevice();
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
	memcpy(data, vectorData, (size_t)size);
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(*mRenderer, size, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mBuffer, mBufferMemory);

	CopyBuffer(commandPool, stagingBuffer, mBuffer, size);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Buffer::Terminate()
{
	VkDevice device = mRenderer->GetDevice();
	vkDestroyBuffer(device, mBuffer, nullptr);
	vkFreeMemory(device, mBufferMemory, nullptr);
}

void EngineV::UniformBuffer::CreateUniformBuffer(VkBufferUsageFlags usage)
{
	CreateBuffer(*mRenderer, mBufferSize, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, mBuffer, mBufferMemory);

	VkDevice device = mRenderer->GetDevice();
	vkMapMemory(device, mBufferMemory, 0, mBufferSize, 0, &mBufferMapped);
}

void EngineV::UniformBuffer::Update()
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	WorldView ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), mRenderer->GetWidth() / mRenderer->GetHeight(), 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	memcpy(mBufferMapped, &ubo, sizeof(ubo));
}

void EngineV::TypedBuffer::Bind(VkCommandBuffer commandBuffer)
{
	if (mUsage == (VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT))
	{
		VkBuffer vertexBuffers[] = { mBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	}
	else if (mUsage == (VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT))
	{
		vkCmdBindIndexBuffer(commandBuffer, mBuffer, 0, VK_INDEX_TYPE_UINT16);
	}
}
