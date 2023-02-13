#pragma once
#include "GLFW/glfw3.h"

class Window
{
public:
	Window();
	Window(int windowWidth, int windowHeight);
	int Initialize();
	[[nodiscard]] int GetBufferWidth() const { return bufferWidth; }
	[[nodiscard]] int GetBufferHeight() const { return bufferHeight; }
	[[nodiscard]] bool GetShouldClose() const { return glfwWindowShouldClose(mainWindow); }
	[[nodiscard]] int GetWidth() const { return width; }
	[[nodiscard]] int GetHeight() const { return height; }
	[[nodiscard]] bool* GetKey() { return key; }
	[[nodiscard]] float GetXChange() ;
	[[nodiscard]] float GetYChange() ;
	[[nodiscard]] int	GetVSyncStatus() const { return VSync; }
	void SetVSyncStatus(const int status) { VSync = status; }
	void SwapBuffers() const { glfwSwapBuffers(mainWindow); }
	void SetShouldClose() const { glfwSetWindowShouldClose(mainWindow, GL_TRUE); }
	~Window();
private:
	GLFWwindow* mainWindow = nullptr;
	int width, height;
	int bufferWidth = 0, bufferHeight = 0;

	int VSync = GLFW_TRUE;

	bool key[1024];

	float lastX = 0, lastY = 0, xChange = 0, yChange = 0;
	bool mouseFirstMoved = true;

	void CreateCallbacks() const;
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void HandleKey(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};


