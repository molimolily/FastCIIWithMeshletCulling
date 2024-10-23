#include "Camera.h"
#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, float aspectRatio, float near, float far)
	: position(position), worldUp(up), yaw(yaw), pitch(pitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), fov(fov), aspectRatio(aspectRatio), near(near), far(far)
{
	// fov‚ðÝ’è‚µ‚Ä‚©‚çaspectRatio‚ðÝ’è‚·‚é
	SetFoV(fov);
	SetAspectRatio(aspectRatio);

	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	if(viewMatrixDirty)
	{
		viewMatrix = glm::lookAt(position, position + front, up);
		viewMatrixDirty = false;
	}
	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	if(projectionMatrixDirty)
	{
		projectionMatrix = glm::frustum(frustumRect.left, frustumRect.right, frustumRect.bottom, frustumRect.top, near, far);
		projectionMatrixDirty = false;
	}
	return projectionMatrix;
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

float Camera::GetYaw() const
{
	return yaw;
}

float Camera::GetPitch() const
{
	return pitch;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 frontVec;
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(frontVec);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	viewMatrixDirty = true;
}

void Camera::SetPosition(const glm::vec3 position)
{
	this->position = position;
	// UpdateCameraVectors();
	viewMatrixDirty = true;
}

void Camera::SetPosition(const float x, const float y, const float z)
{
	glm::vec3 vec(x, y, z);
	this->position = vec;
	// UpdateCameraVectors();
	viewMatrixDirty = true;
}

void Camera::SetWorldUp(const glm::vec3 worldUp)
{
	this->worldUp = worldUp;
	UpdateCameraVectors();
}

void Camera::SetWorldUp(const float x, const float y, const float z)
{
	glm::vec3 vec(x, y, z);
	this->worldUp = vec;
	UpdateCameraVectors();
}

void Camera::SetYaw(const float yaw)
{
	this->yaw = yaw;
	UpdateCameraVectors();
}

void Camera::SetPitch(const float pitch)
{
	this->pitch = std::clamp(pitch, -89.0f, 89.0f);
	UpdateCameraVectors();
}

void Camera::SetFoV(const float fov)
{
	this->fov = fov;
	frustumRect.top = near * tan(glm::radians(fov) / 2);
	frustumRect.bottom = -frustumRect.top;
	SetAspectRatio(aspectRatio);

	projectionMatrixDirty = true;
}

void Camera::SetAspectRatio(const float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	frustumRect.right = frustumRect.top * aspectRatio;
	frustumRect.left = -frustumRect.right;

	projectionMatrixDirty = true;
}

void Camera::SetNearPlane(const float near)
{
	this->near = near;

	projectionMatrixDirty = true;
}

void Camera::SetFarPlane(const float far)
{
	this->far = far;

	projectionMatrixDirty = true;
}

void Camera::SetFrustumRect(const Rect frustumRect)
{
	this->frustumRect = frustumRect;

	projectionMatrixDirty = true;
}

void Camera::SetFrustumRect(const float left, const float right, const float bottom, const float top)
{
	frustumRect.left = left;
	frustumRect.right = right;
	frustumRect.bottom = bottom;
	frustumRect.top = top;

	projectionMatrixDirty = true;
}

void Camera::SetViewportParams(const Viewport viewportParams)
{
	this->viewportParams = viewportParams;
}

void Camera::SetViewportParams(const float x, const float y, const float width, const float height)
{
	viewportParams.x = x;
	viewportParams.y = y;
	viewportParams.width = width;
	viewportParams.height = height;
}

void Camera::SetViewport()
{
	glViewport(viewportParams.x, viewportParams.y, viewportParams.width, viewportParams.height);
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

glm::vec3 Camera::GetUp() const
{
	return up;
}

glm::vec3 Camera::GetFront() const
{
	return front;
}

glm::vec3 Camera::GetRight() const
{
	return right;
}

float Camera::GetNearPlane() const
{
	return near;
}

float Camera::GetFarPlane() const
{
	return far;
}

Rect Camera::GetFrustumRect() const
{
	return frustumRect;
}

FrustumPlanes Camera::GetFrustumPlanes() const
{
	FrustumPlanes planes;
	float top = frustumRect.top;
	float right = frustumRect.right;
	float bottom = frustumRect.bottom;
	float left = frustumRect.left;

	glm::vec3 nearPlane = glm::vec3(0.0f, 0.0f, -1.0f);
	planes.near = glm::vec4(nearPlane, near);

	glm::vec3 farPlane = glm::vec3(0.0f, 0.0f, 1.0f);
	planes.far = glm::vec4(farPlane, far);

	glm::vec3 topPlane =  -(right - left) * glm::vec3(0.0f, near, top);
	topPlane = glm::normalize(topPlane);
	planes.top = glm::vec4(topPlane, 0.0f);

	glm::vec3 bottomPlane = (right - left) * glm::vec3(0.0f, near, bottom);
	bottomPlane = glm::normalize(bottomPlane);
	planes.bottom = glm::vec4(bottomPlane, 0.0f);

	glm::vec3 rightPlane = -(top - bottom) * glm::vec3(near, 0.0f, right);
	rightPlane = glm::normalize(rightPlane);
	planes.right = glm::vec4(rightPlane, 0.0f);

	glm::vec3 leftPlane = (top - bottom) * glm::vec3(near, 0.0f, left);
	leftPlane = glm::normalize(leftPlane);
	planes.left = glm::vec4(leftPlane, 0.0f);

	/*glm::vec3 rt = glm::vec3(right, top, -near);
	glm::vec3 rb = glm::vec3(right, bottom, -near);
	glm::vec3 lt = glm::vec3(left, top, -near);
	glm::vec3 lb = glm::vec3(left, bottom, -near);
	glm::vec3 lb2lt = lt - lb;
	glm::vec3 lb2rb = rb - lb;

	glm::vec3 nearPlaneNormal = glm::normalize(glm::cross(lb2lt, lb2rb));
	planes.near = glm::vec4(nearPlaneNormal, near);

	glm::vec3 farPlaneNormal = -nearPlaneNormal;
	planes.far = glm::vec4(farPlaneNormal, far);

	glm::vec3 topPlaneNormal = glm::normalize(glm::cross(lt, rt));
	planes.top = glm::vec4(topPlaneNormal, 0.0f);

	glm::vec3 bottomPlaneNormal = glm::normalize(glm::cross(rb, lb));
	planes.bottom = glm::vec4(bottomPlaneNormal, 0.0f);

	glm::vec3 rightPlaneNormal = glm::normalize(glm::cross(rt, rb));
	planes.right = glm::vec4(rightPlaneNormal, 0.0f);

	glm::vec3 leftPlaneNormal = glm::normalize(glm::cross(lb, lt));
	planes.left = glm::vec4(leftPlaneNormal, 0.0f);*/

	return planes;
}

Viewport Camera::GetViewportParams() const
{
	return viewportParams;
}

void Camera::Rotate(const float yaw, const float pitch)
{
	this->yaw += yaw;
	this->pitch = std::clamp(this->pitch + pitch, -89.0f, 89.0f);
	UpdateCameraVectors();
}

void Camera::Translate(const glm::vec3 translation)
{
	position += translation;
	// UpdateCameraVectors();

	viewMatrixDirty = true;
}

void Camera::Translate(const float x, const float y, const float z)
{
	glm::vec3 vec(x, y, z);
	position += vec;
	// UpdateCameraVectors();

	viewMatrixDirty = true;
}

void Camera::MoveForward(const float distance)
{
	position += front * distance;
	// UpdateCameraVectors();

	viewMatrixDirty = true;
}

void Camera::MoveUp(const float distance)
{
	position += up * distance;
	// UpdateCameraVectors();

	viewMatrixDirty = true;
}

void Camera::MoveRight(const float distance)
{
	position += right * distance;
	// UpdateCameraVectors();

	viewMatrixDirty = true;
}