#include "iostream"

#include "GL/glew.h"
#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

Window::Window()
{
	width = 800;
	height = 600;

	*key = new bool[1024];

	for (bool & i : key)
	{
		i = false;
	}
}

Window::Window(int windowWidth, int windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	auto* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	theWindow->width = width;
	theWindow->height = height;
	glViewport(0, 0, width, height);
}

void Window::HandleKey(GLFWwindow* window, int key, int code, int action, int mode)
{
	auto* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	const int cursorMode = glfwGetInputMode(theWindow->mainWindow, GLFW_CURSOR);
	if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (cursorMode == GLFW_CURSOR_NORMAL)
		{
			glfwSetInputMode(theWindow->mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			theWindow->mouseFirstMoved = true;
		}
		else
		{
			glfwSetInputMode(theWindow->mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	if (key >= 0 && key <= 1024 && cursorMode == GLFW_CURSOR_DISABLED)
	{
		if (action == GLFW_PRESS)
		{
			theWindow -> key[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow -> key[key] = false;
		}
	}
}

float Window::GetXChange()
{
	const float delta = xChange;
	xChange = 0.0f;
	return delta;
}

float Window::GetYChange()
{
	const float delta = yChange;
	yChange = 0.0f;
	return delta;
}


int Window::Initialize()
{
	
	//initialize GLFW
	if (glfwInit() == GL_FALSE)
	{
		cout << "GLFW initialization failed !" << endl;
		glfwTerminate();
		return 1;
	}

	//setup GLFW window properties
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	//Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "OpenGL test window", nullptr, nullptr);
	if (!mainWindow)
	{
		cout << "GLFW window creation failed!" << endl;
		glfwTerminate();
		return 1;
	}

	//set window icon
	GLFWimage image[2];

	image[0].pixels = stbi_load("icon/console-64px.png", &image[0].width, &image[0].height, nullptr, 4);
	image[1].pixels = stbi_load("icon/console-512px.png", &image[1].width, &image[1].height, nullptr, 4);

	glfwSetWindowIcon(mainWindow, 2, image);
	//get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//handle call back
	CreateCallbacks();

	//lock mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	//allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "Glew initialization failed!" << endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//Vertical sync
	glfwSwapInterval(GLFW_TRUE);

	//setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

void Window::HandleMouse(GLFWwindow* window, const double xPos, const double yPos)
{
	auto* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (glfwGetInputMode(theWindow->mainWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		if (theWindow->mouseFirstMoved)
		{
			theWindow->lastX = static_cast<float>(xPos);
			theWindow->lastY = static_cast<float>(yPos);
			theWindow->mouseFirstMoved = false;
		}

		theWindow->xChange = static_cast<float>(xPos) - theWindow->lastX;
		theWindow->yChange = theWindow->lastY - static_cast<float>(yPos);

		theWindow->lastX = static_cast<float>(xPos);
		theWindow->lastY = static_cast<float>(yPos);
	}
}

void Window::DisplayError(const int code, const char* description)
{
	cout << "Error: " << code << " " << description << endl;
}


void Window::CreateCallbacks() const
{
	//error callback
	//glfwSetErrorCallback(DisplayError);
	//handle key
	glfwSetKeyCallback(mainWindow, HandleKey);
	//handle window resize
	glfwSetFramebufferSizeCallback(mainWindow, FramebufferSizeCallback);
	//handle mouse
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
