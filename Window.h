#pragma once
#include "iostream"

#include "GLFW/glfw3.h"

using namespace std;

class Window
{
public:
	Window();
	Window(int windowWidth, int windowHeight);
	int Initialize();
	[[nodiscard]] int GetBufferWidth() const { return bufferWidth; }
	[[nodiscard]] int GetBufferHeight() const { return bufferHeight; }

	[[nodiscard]] bool GetShouldClose() const { return glfwWindowShouldClose(mainWindow); }
	void SwapBuffers() const { glfwSwapBuffers(mainWindow); }
	~Window();
private:
	GLFWwindow* mainWindow = nullptr;
	int width, height;
	int bufferWidth = 0, bufferHeight = 0;
};


