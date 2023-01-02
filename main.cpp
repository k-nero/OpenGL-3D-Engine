#include "GL\glew.h"
#include "iostream"
#include "GLFW/glfw3.h"

const GLint width = 800;
const GLint height = 600;

int main()
{
	if(!glfwInit()) 
	{
		printf("GLFW initialization failed !");
		glfwTerminate();
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* mainWindow = glfwCreateWindow(width, height, "Test window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	int bufferWidth;
	int bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	
	glfwMakeContextCurrent(mainWindow);
	
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Glew initialization falied!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}
	
	glViewport(0, 0, bufferWidth, bufferHeight);
	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();
		
		//Clear the window
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}
