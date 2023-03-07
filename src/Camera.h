#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class Camera
{
public:
	Camera();
	Camera(vec3 initCameraPos, vec3 initUp, float initYaw, float initPitch, float initMovementSpeed, float initMouseSensitivity);
	void KeyControl(bool* keys, float deltaTime);
	void MouseControl(float xChange, float yChange);
	[[nodiscard]] mat4 CalculateViewMatrix() const;
	[[nodiscard]] vec3 GetCameraPosition() const;
	[[nodiscard]] vec3 GetCameraDirection() const;
	~Camera();
private:
	vec3 cameraPos = vec3();
	vec3 front = vec3();
	vec3 up = vec3();
	vec3 right = vec3();
	vec3 worldUp = vec3();

	float yaw = 0.0f;
	float pitch = 0.0f;

	float movementSpeed = 0.0f;
	float mouseSensitivity = 0.0f;

	void Update();
};