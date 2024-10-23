#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "Shader.h"

extern std::string basePath;

class FrustumRenderer
{
public:
	FrustumRenderer();
	~FrustumRenderer();

	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	void UpdateMesh(const Camera& camera);
	void Draw(Camera &camera);
private:
	GLuint vao, vbo, ebo;
	Shader shader;

	void SetupMesh();
};