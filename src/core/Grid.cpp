#include "Grid.h"
#include <iostream>

Grid::Grid(const float lineWidth, const float pitch, const std::string& vertPath, const std::string& fragPath)
	: lineWidth(lineWidth), pitch(pitch), shader(vertPath, fragPath)
{
	SetupMesh();
	shader.Use();
	shader.SetFloat("lineWidth", lineWidth);
	shader.SetFloat("pitch", pitch);

}

Grid::~Grid()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Grid::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// í∏ì_ç¿ïW
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	// uvç¿ïW
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
}

void Grid::Draw(Camera& camera)
{
	shader.Use();

	glm::mat4 vp = camera.GetProjectionMatrix() * camera.GetViewMatrix();
	shader.SetMat4("vp", vp);

	shader.SetVec3("pos", camera.GetPosition());
	shader.SetVec3("front", camera.GetFront());
	shader.SetVec3("up", camera.GetUp());
	shader.SetVec3("right", camera.GetRight());

	Rect rect = camera.GetFrustumRect();
	float near = camera.GetNearPlane();
	float far = camera.GetFarPlane();
	shader.SetVec4("rect", rect.left, rect.right, rect.bottom, rect.top);
	shader.SetFloat("near", near);
	shader.SetFloat("far", far);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

