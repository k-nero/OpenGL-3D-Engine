#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

using namespace glm;

class Camera
{
public:
	Camera();
	Camera(vec3 initCameraPos, vec3 initUp, float initYaw, float initPitch, float initMovementSpeed, float initMouseSensitivity);
	void KeyControl(bool* keys, float deltaTime);
	void MouseControl(float xChange, float yChange);
	[[nodiscard]] mat4 CalculateViewMatrix() const;
	~Camera();
private:
	vec3 cameraPos;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;

	void Update();
};

