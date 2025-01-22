#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> gValidationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication
{
public:
	void run()
	{
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	void initVulkan()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

		createInstance();
	}
	
	void createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			throw std::runtime_error("validation layers requested, but not available");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		createInfo.enabledLayerCount = 0;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance");
		}
	}

	bool checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : gValidationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(mWindow))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		vkDestroyInstance(mInstance, nullptr);

		glfwDestroyWindow(mWindow);

		glfwTerminate();
	}

private:
	GLFWwindow* mWindow = nullptr;
	VkInstance mInstance;
};

int main()
{
	HelloTriangleApplication app;

	try 
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}