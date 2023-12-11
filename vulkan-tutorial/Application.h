#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Application
{
public:
	void Run();

private:
	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void Cleanup();

private:
	void CreateInstance();
	void SetupDebugMessenger();

private:
	GLFWwindow* m_pWindow;
	VkInstance m_vInstance;
	VkDebugUtilsMessengerEXT m_DebugMessenger;
};