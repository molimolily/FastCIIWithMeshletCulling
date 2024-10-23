#include "Transform.h"

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : position(position), rotation(rotation), scale(scale), 
    modelMatrix(), invTransModelMatrix(), isModelMatrixDirty(true), isInvTransModelMatrixDirty(true)
{}

glm::mat4 Transform::GetModelMatrix()
{
    if (isModelMatrixDirty)
    {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::scale(modelMatrix, scale);
        isModelMatrixDirty = false;
        isInvTransModelMatrixDirty = true;
    }
    return modelMatrix;
}

glm::mat4 Transform::GetInvTransModelMatrix()
{
    if (isInvTransModelMatrixDirty)
    {
        invTransModelMatrix = glm::transpose(glm::inverse(GetModelMatrix()));
        isInvTransModelMatrixDirty = false;
    }
    return invTransModelMatrix;
}

void Transform::SetPosition(glm::vec3 position)
{
    this->position = position;
    isModelMatrixDirty = true;
}

void Transform::SetPosition(float x, float y, float z)
{
	SetPosition(glm::vec3(x, y, z));
}

void Transform::SetRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
    isModelMatrixDirty = true;
}

void Transform::SetRotation(float x, float y, float z)
{
	SetRotation(glm::vec3(x, y, z));
}

void Transform::SetScale(glm::vec3 scale)
{
    this->scale = scale;
    isModelMatrixDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
    SetScale(glm::vec3(x, y, z));
}

void Transform::Translate(glm::vec3 translation)
{
    position += translation;
    isModelMatrixDirty = true;
}

void Transform::Translate(float x, float y, float z)
{
	Translate(glm::vec3(x, y, z));
}

void Transform::Rotate(glm::vec3 rotation)
{
    this->rotation += rotation;
    isModelMatrixDirty = true;
}

void Transform::Rotate(float x, float y, float z)
{
	Rotate(glm::vec3(x, y, z));
}

void Transform::UniformScaling(float scale)
{
	this->scale = glm::vec3(scale);
	isModelMatrixDirty = true;
}

glm::vec3 Transform::Position() const
{
	return position;
}

glm::vec3 Transform::Rotation() const
{
	return rotation;
}

glm::vec3 Transform::Scale() const
{
	return scale;
}