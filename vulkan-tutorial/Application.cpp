#include <pch.h>
#include "Application.h"

const std::vector<const char*> g_ValidationLayers{
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool g_EnableValidationLayers{ false };
#else
const bool g_EnableValidationLayers{ true };
#endif

struct QueueFamilyIndices
{
	std::optional<uint32_t> GraphicsFamily;
	
	bool IsComplete() { return GraphicsFamily.has_value(); }
		
};

#pragma region Validation Layer Stuff
bool CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : g_ValidationLayers) 
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
			return false;
	}
	return true;
}

VkResult CreateDebugUtilsMessagerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT* const pCreateInfo,
	VkAllocationCallbacks const* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func)
		func(instance, debugMessenger, pAllocator);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const*      pCallbackData,
    void*                                            pUserData)
{
	std::cerr << "Validation layer: " <<
		pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = 
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;}

std::vector<const char*> GetRequiredExtensions()
{
	uint32_t glfwExtensionsCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);
	if (g_EnableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}
#pragma endregion


#pragma region Queue Family Stuff
QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices{};

	uint32_t queueFamilyCount{};
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies{queueFamilyCount};
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (VkQueueFamilyProperties const& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.GraphicsFamily = i;

		if (indices.IsComplete())
			break;

		++i;
	}

	return indices;
}
#pragma endregion

#pragma region Physical Device Stuff
bool IsDeviceSuitable(VkPhysicalDevice device)
{
	//VkPhysicalDeviceProperties deviceProps{};
	//VkPhysicalDeviceFeatures deviceFeats{};
	//vkGetPhysicalDeviceProperties(device, &deviceProps);
	//vkGetPhysicalDeviceFeatures(device, &deviceFeats);
	//
	//return	deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
	//		deviceFeats.geometryShader;

	QueueFamilyIndices indices = FindQueueFamilies(device);
	return indices.IsComplete();
}

int RateDeviceSuitability(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProps{};
	VkPhysicalDeviceFeatures deviceFeats{};
	vkGetPhysicalDeviceProperties(device, &deviceProps);
	vkGetPhysicalDeviceFeatures(device, &deviceFeats);

	int score = 0;
	
	// Discrete GPUs have significant performance advantage
	if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	// Max possible size of textures affect graphics quality
	score += deviceProps.limits.maxImageDimension2D;

	// Application can't function without geometyry shaders
	if (!deviceFeats.geometryShader)
		score = 0;

	return score;
}
#pragma endregion

void Application::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void Application::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Telling glfw NOT to create OGL context.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_pWindow = glfwCreateWindow(800, 600, "Vulkan Tutorial", nullptr, nullptr);
}

void Application::InitVulkan()
{
	CreateInstance();
	SetupDebugMessenger();
	PickPhysicalDevice();
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(m_pWindow))
	{
		glfwPollEvents();

	}
}

void Application::Cleanup()
{
	if (g_EnableValidationLayers)
		DestroyDebugUtilsMessengerEXT(m_vInstance, m_DebugMessenger, nullptr);
	vkDestroyInstance(m_vInstance, nullptr);
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

// Helper stuff
void Application::CreateInstance()
{
	if (g_EnableValidationLayers && !CheckValidationLayerSupport())
		throw std::runtime_error("Validation layers requested, but not available!");

	// Optional stuff, but is good to just be explicit, so it's more optimal for your device
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Is compulsory
	// Tell vulkan driver which gloabl ext and validation layers to use 
	// (global meaning ENTIRE program and not just specific device)
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Specifying global extensions. 
	// Vulkan is an agnostic API, need the extension to interface with the window system.
	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = (uint32_t)extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = 0;
	createInfo.pNext = nullptr;

	// Debug utils
	if (g_EnableValidationLayers)
	{
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		createInfo.enabledLayerCount = (uint32_t)g_ValidationLayers.size();
		createInfo.ppEnabledLayerNames = g_ValidationLayers.data();

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;	
	}
	
	// Creating vulkan instance
	// The second param is custom allocator callbacks
	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vInstance);
	if (result != VK_SUCCESS)
		throw std::runtime_error{ "Failed to create vk instance!" };

	// Misc.
	// Checking extension support
	//uint32_t extensionCount = 0;
	//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	//
	//std::vector<VkExtensionProperties> extensions(extensionCount);
	//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	//for (const auto& ee : extensions) {
	//	std::cout << '\t' << ee.extensionName << '\n';
	//}
}

void Application::SetupDebugMessenger()
{
	if (!g_EnableValidationLayers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessagerEXT(m_vInstance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
		throw std::runtime_error("Failed to set up debug messenger!");
}

void Application::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_vInstance, &deviceCount, nullptr);

	if (!deviceCount)
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_vInstance, &deviceCount, devices.data());

	// More straight forward version for teh sake of this tutorial
	for (VkPhysicalDevice const& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			m_PhysicalDevice = device;
			break;
		}
	}

	if (m_PhysicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("Failed to find suitable GPU!");


	{ // Example using a score system to identify best graphgics card.
		// Finding best quality graphics card
		// Multimap will autosort candidates by increasing score
		//std::multimap<int, VkPhysicalDevice> candidates{};
		//
		//for (VkPhysicalDevice const& device : devices)
		//{
		//	int score = RateDeviceSuitability(device);
		//	candidates.insert(std::make_pair(score, device));
		//}

		//if (candidates.rbegin()->first > 0)
		//	m_PhysicalDevice = candidates.rbegin()->second;
		//else
		//	throw std::runtime_error("Failed to find suitable GPU!");
	}
}