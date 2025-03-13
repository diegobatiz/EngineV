#include "Precompiled.h"
#include "Buffer.h"

#include "Renderer.h"
#include "BufferCreation.h"

using namespace EngineV;

void Buffer::Initalize(const Renderer& renderer, const CommandPool& commandPool, VkBufferUsageFlags usage, const void* vectorData, VkDeviceSize size)
{
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

template<class DataType>
void UniformBuffer<DataType>::Initialize(const Renderer& renderer, VkBufferUsageFlags usage)
{
	mRenderer = &renderer;
	VkDeviceSize bufferSize = sizeof(DataType);

	CreateBuffer(*mRenderer, bufferSize, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,mBuffer, mBuffer);

	VkDevice device = mRenderer->GetDevice();
	vkMapMemory(device, mBufferMemory, 0, bufferSize, 0, &mBuffersMapped);
}
