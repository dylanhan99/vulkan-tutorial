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
	void PickPhysicalDevice();

private:
	GLFWwindow* m_pWindow{nullptr};
	VkInstance m_vInstance{VK_NULL_HANDLE};
	VkDebugUtilsMessengerEXT m_DebugMessenger{VK_NULL_HANDLE};
	VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE}; // This is the graphics card
};