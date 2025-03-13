#pragma once
#include "CommandPool.h"

namespace EngineV
{
	class Renderer;

	class Buffer
	{
	public:
		Buffer(const Renderer& renderer);
		void Initalize(const CommandPool& commandPool, VkBufferUsageFlags usage, const void* data = nullptr, VkDeviceSize size = 0);
		void Terminate();

	private:
		const Renderer* mRenderer = nullptr;
		VkBuffer mBuffer;
		VkDeviceMemory mBufferMemory;

	public:
		template <class DataType>
		void Initialize(const CommandPool& commandPool, VkBufferUsageFlags usage, const std::vector<DataType>& data)
		{
			VkDeviceSize size = sizeof(data[0] * data.size());
			Initialize(commandPool, usage, data.data(), size);
		}
	};
}
