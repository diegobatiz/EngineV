#pragma once
#include "CommandPool.h"

namespace EngineV
{
	class Renderer;

	class Buffer
	{
	public:
		Buffer() = default;
		virtual void Terminate();
		VkBuffer GetBuffer() { return mBuffer; }
		VkBuffer GetBuffer() const { return mBuffer; }

	protected:
		void Initalize(const Renderer& renderer, const CommandPool& commandPool, VkBufferUsageFlags usage, const void* data, VkDeviceSize size);

		const Renderer* mRenderer = nullptr;
		VkBuffer mBuffer;
		VkDeviceMemory mBufferMemory;
		VkBufferUsageFlags mUsage;
	};

	class TypedBuffer final : public Buffer
	{
	public:
		template<class DataType>
		void Initialize(const Renderer& renderer, const CommandPool& commandPool, VkBufferUsageFlags usage, const std::vector<DataType> data)
		{
			VkDeviceSize size = sizeof(data[0])* data.size();
			TypedBuffer::Initalize(renderer, commandPool, usage, data.data(), size);
		}

		void Bind(VkCommandBuffer commandBuffer);
	};
	

	class UniformBuffer : public Buffer
	{
	public:
		template<class DataType>
		void Initialize(const Renderer& renderer, VkBufferUsageFlags usage)
		{
			mRenderer = &renderer;
			mBufferSize = sizeof(DataType);

			CreateUniformBuffer(usage);
		}

		virtual void Update();

	private:
		void CreateUniformBuffer(VkBufferUsageFlags usage);
		VkDeviceSize mBufferSize;
		void* mBufferMapped;
	};
}
