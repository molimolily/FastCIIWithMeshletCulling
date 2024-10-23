#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Viewport
{
	float x;
	float y;
	float width;
	float height;
};

struct Rect
{
	float left;
	float right;
	float bottom;
	float top;
};

// vec4(N, d): dot(N, x) = d
struct FrustumPlanes
{
	glm::vec4 left;
	glm::vec4 right;
	glm::vec4 bottom;
	glm::vec4 top;
	glm::vec4 near;
	glm::vec4 far;
};

class Camera
{
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	bool viewMatrixDirty;
	bool projectionMatrixDirty;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	
	float fov;
	float aspectRatio;

	Rect frustumRect;
	float near;
	float far;

	Viewport viewportParams;
public:
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		float yaw = -90.0f, 
		float pitch = 0.0f, 
		float fov = 45.0f, 
		float aspectRatio = 16.0f / 9.0f, 
		float near = 0.3f, 
		float far = 100.0f
	);

	void UpdateCameraVectors();

	void SetPosition(const glm::vec3 position);
	void SetPosition(const float x, const float y, const float z);
	void SetWorldUp(const glm::vec3 worldUp);
	void SetWorldUp(const float x, const float y, const float z);
	void SetYaw(const float yaw);
	void SetPitch(const float pitch);
	void SetFoV(const float fov);
	void SetAspectRatio(const float aspectRatio);
	void SetNearPlane(const float near);
	void SetFarPlane(const float far);
	void SetFrustumRect(const Rect frustumRect);
	void SetFrustumRect(const float left, const float right, const float bottom, const float top);
	void SetViewportParams(const Viewport viewportParams);
	void SetViewportParams(const float x, const float y, const float width, const float height);
	void SetViewport();

	glm::vec3 GetPosition() const;
	glm::vec3 GetFront() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	Rect GetFrustumRect() const;
	FrustumPlanes GetFrustumPlanes() const;
	Viewport GetViewportParams() const;


	void Rotate(const float yaw, const float pitch);
	void Translate(const glm::vec3 translation);
	void Translate(const float x, const float y, const float z);
	void MoveForward(const float distance);
	void MoveUp(const float distance);
	void MoveRight(const float distance);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewProjectionMatrix();
};
