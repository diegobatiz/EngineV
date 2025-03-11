#pragma once

#include "Precompiled.h"

namespace EngineV
{
	struct Vertex
	{
		virtual VkVertexInputBindingDescription GetBindingDescription() const = 0;
		virtual std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() const = 0;
	};

	struct VertexPCT : public Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		VkVertexInputBindingDescription GetBindingDescription() const override
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VertexPCT);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() const override
		{
			std::array<VkVertexInputAttributeDescription, 3> attributeDesciptions{};
			attributeDesciptions[0].binding = 0;
			attributeDesciptions[0].location = 0;
			attributeDesciptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDesciptions[0].offset = offsetof(VertexPCT, pos);

			attributeDesciptions[1].binding = 0;
			attributeDesciptions[1].location = 1;
			attributeDesciptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDesciptions[1].offset = offsetof(VertexPCT, color);

			attributeDesciptions[2].binding = 0;
			attributeDesciptions[2].location = 2;
			attributeDesciptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDesciptions[2].offset = offsetof(VertexPCT, texCoord);

			return attributeDesciptions;
		}
	};
}