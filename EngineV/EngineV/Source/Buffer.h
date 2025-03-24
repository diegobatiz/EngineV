#pragma once
#include "CommandPool.h"
#include "BufferCreation.h"

namespace EngineV
{
	class Renderer;

	class Buffer
	{
	public:
		Buffer() = default;
		virtual void Initalize(const Renderer& renderer, const CommandPool& commandPool, VkBufferUsageFlags usage, const void* data, VkDeviceSize size);
		virtual void Terminate();

	protected:
		const Renderer* mRenderer = nullptr;
		VkBuffer mBuffer;
		VkDeviceMemory mBufferMemory;
	};

	template<class DataType>
	class TypedBuffer final : public Buffer
	{
	public:
		void Initialize(const Renderer& renderer, const CommandPool& commandPool, VkBufferUsageFlags usage, const std::vector<DataType> data)
		{
			VkDeviceSize size = sizeof(data[0])* data.size();
			TypedBuffer::Initalize(renderer, commandPool, usage, data.data(), size);
		}
	};
	
	template<class DataType>
	class UniformBuffer final : public Buffer
	{
	public:
		void Initialize(const Renderer& renderer, VkBufferUsageFlags usage)
		{
			mRenderer = &renderer;
			VkDeviceSize bufferSize = sizeof(DataType);

			CreateBuffer(*mRenderer, bufferSize, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, mBuffer, mBufferMemory);

			VkDevice device = mRenderer->GetDevice();
			vkMapMemory(device, mBufferMemory, 0, bufferSize, 0, &mBufferMapped);
		}
	
	private:
		void* mBufferMapped;
	};
}
