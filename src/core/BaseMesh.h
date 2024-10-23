#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <meshoptimizer.h>
#include "DrawIndirectCommand.h"
#include "MultiDrawIndirectCommand.h"

struct alignas(16) Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class BaseMesh
{
public:
	BaseMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
		: vertices(std::move(vertices)), indices(std::move(indices))
	{
	}

	virtual ~BaseMesh()
	{
		if (isUploaded)
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
			isUploaded = false;
		}
	}

	virtual void Draw() const
	{
		if(!isUploaded)
		{
			std::cerr << "Mesh has not been uploaded to GPU" << std::endl;
			return;
		}

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	virtual void DrawInstanced(unsigned int instanceCount) const
	{
		if (!isUploaded)
		{
			std::cerr << "Mesh has not been uploaded to GPU" << std::endl;
			return;
		}

		glBindVertexArray(vao);
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
		glBindVertexArray(0);
	}

	virtual void DrawIndirect(const class DrawIndirectCommand& command)
	{
		if (!isUploaded)
		{
			std::cerr << "Mesh has not been uploaded to GPU" << std::endl;
			return;
		}

		glBindVertexArray(vao);
		command.Bind();
		glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	virtual void MultiDrawIndirect(const class MultiDrawIndirectCommand& command)
	{
		if (!isUploaded)
		{
			std::cerr << "Mesh has not been uploaded to GPU" << std::endl;
			return;
		}

		glBindVertexArray(vao);
		command.Bind();
		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, command.GetDrawCount(), 0);
		glBindVertexArray(0);
	}

	virtual unsigned int GetVertexCount() const
	{
		return vertices.size();
	}

	virtual unsigned int GetIndexCount() const
	{
		return indices.size();
	}

	virtual std::vector<Vertex> GetVertices() const
	{
		return vertices;
	}

	virtual std::vector<GLuint> GetIndices() const
	{
		return indices;
	}

	virtual void SetVertices(const std::vector<Vertex>& newVertices)
	{
		vertices = newVertices;
		UploadVertices();
	}

	virtual void SetIndices(const std::vector<GLuint>& newIndices)
	{
		indices = newIndices;
		UploadIndices();
	}

protected:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	bool isUploaded = false; // is mesh uploaded to GPU

	virtual void Init() = 0;

	virtual void SetupMesh()
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);

		isUploaded = true;
	}

	virtual void OptimizeMesh()
	{
		std::vector<unsigned int> remap(indices.size());
		auto vertexCount = meshopt_generateVertexRemap(remap.data(), indices.data(), indices.size(), vertices.data(), vertices.size(), sizeof(Vertex));
		std::vector<Vertex> newVertices(vertexCount);
		std::vector<GLuint> newIndices(indices.size());
		meshopt_remapVertexBuffer(newVertices.data(), vertices.data(), vertices.size(), sizeof(Vertex), remap.data());
		meshopt_remapIndexBuffer(newIndices.data(), indices.data(), indices.size(), remap.data());
		remap.clear();
		remap.shrink_to_fit();
		vertices = std::move(newVertices);
		indices = std::move(newIndices);
		meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());
		meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indices.size(), vertices.data(), vertices.size(), sizeof(Vertex));
	}

	virtual void UploadVertices()
	{
		if(!isUploaded)
		{
			std::cerr << "vbo has not been generated yet" << std::endl;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	virtual void UploadIndices()
	{
		if (!isUploaded)
		{
			std::cerr << "ebo has not been generated yet" << std::endl;
			return;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};
