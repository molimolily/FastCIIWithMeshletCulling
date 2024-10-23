#include "FrustumRenderer.h"

FrustumRenderer::FrustumRenderer() : shader(basePath + "shaders\\frustum.vert", basePath + "shaders\\frustum.frag")
{
	SetupMesh();
}

FrustumRenderer::~FrustumRenderer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void FrustumRenderer::Draw(Camera& camera)
{
	shader.Use();
	shader.SetVec4("color", color);
	shader.SetMat4("vp", camera.GetViewProjectionMatrix());

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void FrustumRenderer::SetupMesh()
{
	std::vector<glm::vec3> vertices = {
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f)
	};

	std::vector<unsigned int> indices = {
		0, 1, 1, 3, 3, 2, 2, 0, // near plane
		4, 5, 5, 7, 7, 6, 6, 4, // far plane
		0, 4, 1, 5, 2, 6, 3, 7 // side
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void FrustumRenderer::UpdateMesh(const Camera& camera)
{
	std::vector<glm::vec3> vertices;

	glm::vec3 pos = camera.GetPosition();
	glm::vec3 front = camera.GetFront();
	glm::vec3 up = camera.GetUp();
	glm::vec3 right = camera.GetRight();

	float near = camera.GetNearPlane();
	float far = camera.GetFarPlane();
	Rect rect = camera.GetFrustumRect();

	// near plane
	glm::vec3 ntl = pos + front * near + up * rect.top + right * rect.left;
	glm::vec3 ntr = pos + front * near + up * rect.top + right * rect.right;
	glm::vec3 nbl = pos + front * near + up * rect.bottom + right * rect.left;
	glm::vec3 nbr = pos + front * near + up * rect.bottom + right * rect.right;

	// far plane
	glm::vec3 ftl = pos + front * far + (up * rect.top + right * rect.left) * far / near;
	glm::vec3 ftr = pos + front * far + (up * rect.top + right * rect.right) * far / near;
	glm::vec3 fbl = pos + front * far + (up * rect.bottom + right * rect.left) * far / near;
	glm::vec3 fbr = pos + front * far + (up * rect.bottom + right * rect.right) * far / near;

	// near plane
	vertices.push_back(ntl);
	vertices.push_back(ntr);
	vertices.push_back(nbl);
	vertices.push_back(nbr);

	// far plane
	vertices.push_back(ftl);
	vertices.push_back(ftr);
	vertices.push_back(fbl);
	vertices.push_back(fbr);

	// Uploading vertices to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}