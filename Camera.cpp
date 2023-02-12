#include "Camera.h"

#include "Gl\glew.h"
#include "GLFW/glfw3.h"

Camera::Camera()
{
	
}

Camera::Camera(vec3 initCameraPos, vec3 initUp, float initYaw, float initPitch, float initMovementSpeed, float initMouseSensitivity)
{
	this->cameraPos = initCameraPos;
	this->worldUp = initUp;
	this->yaw = initYaw;
	this->pitch = initPitch;
	this->front = vec3(0.0f, 0.0f, -1.0f);
	this->movementSpeed = initMovementSpeed;
	this->mouseSensitivity = initMouseSensitivity;

	Update();
}

void Camera::KeyControl(bool* keys, float deltaTime)
{
	const float velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		cameraPos += front * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		cameraPos -= front * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		cameraPos -= right * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		cameraPos += right * velocity;
	}
	if (keys[GLFW_KEY_SPACE])
	{
		cameraPos += up * velocity;
	}
	if (keys[GLFW_KEY_LEFT_SHIFT])
	{
		cameraPos -= up * velocity;
	}
}

mat4 Camera::CalculateViewMatrix() const
{
	return lookAt(cameraPos, cameraPos + front, up);
}

void Camera::MouseControl(float xChange, float yChange)
{
	xChange *= mouseSensitivity;
	yChange *= mouseSensitivity;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	Update();
}

void Camera::Update()
{
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = normalize(front);

	right = normalize(cross(front, worldUp));
	up = normalize(cross(right, front));
}

Camera::~Camera()
= default;
