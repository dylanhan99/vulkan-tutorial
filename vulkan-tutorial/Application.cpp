#include <pch.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::cout << extensionCount << " extensions supported\n";

}

void Application::InitVulkan()
{

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
	glfwDestroyWindow(m_pWindow);

	glfwTerminate();
}
