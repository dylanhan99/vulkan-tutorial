#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

struct QueueFamilyIndices;

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
	void CreateSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();

private:
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	bool IsDeviceSuitable(VkPhysicalDevice device);

private:
	GLFWwindow* m_pWindow{nullptr};
	VkInstance m_vInstance{VK_NULL_HANDLE};
	VkDebugUtilsMessengerEXT m_DebugMessenger{VK_NULL_HANDLE};
	VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE}; // This is the graphics card
	VkDevice m_LogicalDevice{ VK_NULL_HANDLE }; // interface between application and GPU
	VkSurfaceKHR m_Surface{ VK_NULL_HANDLE };

	VkQueue m_GraphicsQueue{ VK_NULL_HANDLE };
	VkQueue m_PresentQueue{ VK_NULL_HANDLE };
};