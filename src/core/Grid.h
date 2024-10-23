#pragma once

#include "Shader.h"
#include "Camera.h"

extern std::string basePath;

class Grid
{
public:

	static constexpr float vertices[] = {
		-1.0, 1.0, 0.0, 0.0, 1.0,
		-1.0, -1.0, 0.0, 0.0, 0.0,
		1.0, -1.0, 0.0, 1.0, 0.0,
		1.0, 1.0, 0.0, 1.0, 1.0
	};

	static constexpr unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Grid(const float lineWidth = 0.05f, const float pitch = 2.0f,
		const std::string& vertPath = basePath + "shaders\\grid.vert", const std::string& fragPath = basePath + "shaders\\grid.frag");
	~Grid();

	void Draw(Camera& camera);

private:
	GLuint VAO, VBO, EBO;
	Shader shader;
	float lineWidth, pitch;
	void SetupMesh();
};
