#pragma once

namespace EngineV
{
	class Window;

#ifdef NDEBUG
	const bool gEnableValidationLayers = false;
#else
	const bool gEnableValidationLayers = true;
#endif

	const std::vector<const char*> gValidationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	class Renderer
	{
	public:
		Renderer(const char* appName, const Window& window);
		~Renderer();

		void Initialize();
		void Terminate();

	private:
		void CreateInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();

		void SetupDebugMessenger();
		void CreateSurface();

	private:
		const char* mAppName;
		const Window* mWindow;

		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		VkSurfaceKHR mSurface;
	};
}