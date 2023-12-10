#include <pch.h>
#include "Application.h"

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
	vkDestroyInstance(m_vInstance, nullptr);
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

// Helper stuff
void Application::CreateInstance()
{
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
	uint32_t glfwExtensionCount{};
	const char** glfwExtensions{};
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	// Creating vulkan instance
	// The second param is custom allocator callbacks
	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vInstance);
	if (result != VK_SUCCESS)
		throw std::runtime_error{ "Failed to create vk instance!" };

	// Misc.
	// Checking extension support
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	for (const auto& ee : extensions) {
		std::cout << '\t' << ee.extensionName << '\n';
	}
}