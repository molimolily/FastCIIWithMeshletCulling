#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
public:   

    glm::mat4 modelMatrix;
    glm::mat4 invTransModelMatrix;
    bool isModelMatrixDirty;
    bool isInvTransModelMatrixDirty;

    Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

    glm::mat4 GetModelMatrix();
    glm::mat4 GetInvTransModelMatrix();

    void SetPosition(glm::vec3 position);
    void SetPosition(float x, float y, float z);
    void SetRotation(glm::vec3 rotation);
    void SetRotation(float x, float y, float z);
    void SetScale(glm::vec3 scale);
    void SetScale(float x, float y, float z);

    void Translate(glm::vec3 translation);
    void Translate(float x, float y, float z);
    void Rotate(glm::vec3 rotation);
    void Rotate(float x, float y, float z);
    void UniformScaling(float scale);

    glm::vec3 Position() const;
    glm::vec3 Rotation() const;
    glm::vec3 Scale() const;
};