#pragma once

namespace EngineV
{
	class Renderer;
	class CommandPool;

	class Texture
	{
	public:
		Texture(const Renderer& renderer);
		void Initalize(const CommandPool& commandPool);
		void Terminate();
		VkImageView GetImageView() const { return mTextureImageView; }
		VkSampler GetSampler() const { return mTextureSampler; }

	private:
		void CreateTextureImage(const CommandPool& commandPool);
		void CreateTextureImageView();
		void CreateTextureSampler();
		
	private:
		const Renderer* mRenderer = nullptr;
		VkImage mTextureImage;
		VkDeviceMemory mTextureImageMemory;
		VkImageView mTextureImageView;
		VkSampler mTextureSampler;
	};
}
