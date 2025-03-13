#pragma once

#include "Renderer.h"
#include "CommandPool.h"

namespace EngineV
{
	void CreateBuffer(const Renderer& renderer, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	
	void CopyBufferToImage(const CommandPool& commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	
	void CopyBuffer(const CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
}