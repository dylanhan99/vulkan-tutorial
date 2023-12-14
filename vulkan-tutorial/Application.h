#pragma once

#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

struct QueueFamilyIndices;
struct SwapChainSupportDetails;

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
	void CreateSwapChain();

private:
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	bool IsDeviceSuitable(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR const& capabilities);


private:
	GLFWwindow* m_pWindow{nullptr};
	VkInstance m_vInstance{VK_NULL_HANDLE};
	VkDebugUtilsMessengerEXT m_DebugMessenger{VK_NULL_HANDLE};
	VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE}; // This is the graphics card
	VkDevice m_LogicalDevice{ VK_NULL_HANDLE }; // interface between application and GPU
	VkSurfaceKHR m_Surface{ VK_NULL_HANDLE };
	VkSwapchainKHR m_SwapChain{ VK_NULL_HANDLE };
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;

	VkQueue m_GraphicsQueue{ VK_NULL_HANDLE };
	VkQueue m_PresentQueue{ VK_NULL_HANDLE };

};