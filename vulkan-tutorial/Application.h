#pragma once

struct GLFWwindow;

class Application
{
public:
	void Run();

	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void Cleanup();

private:
	GLFWwindow* m_pWindow;
};