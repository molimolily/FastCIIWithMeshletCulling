#pragma once
#include "BaseMesh.h"
#include "Mesh.h"
#include <GPUBuffer.h>

struct alignas(16) BoundingSphere
{
	glm::vec3 center;
	float radius;

	glm::vec3 coneAxis;
	float coneCutOff;
};

class MeshletMesh : public BaseMesh
{
public:

	enum class BindingType
	{
		VBO,
		SSBO
	};

	std::vector<meshopt_Meshlet> meshlets;
	std::vector<unsigned int> meshletVertices;
	std::vector<unsigned char> meshletPrimitives;
	std::vector<GLuint> indexOffsets;
	size_t meshletCount;

	MeshletMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	MeshletMesh(const std::shared_ptr<Mesh>& mesh);

	MultiDrawIndirectCommand GenerateMultiDrawIndirectCommand(GLuint instanceCount = 1) const;

	void RemapIndicesForMeshlets();
	void GenetateMeshletBounds(std::vector<BoundingSphere> &bounds);

private:
	const size_t maxVerticesPerMeshlet = 64;
	const size_t maxPrimitivesPerMeshlet = 124;
	const float coneWeight = 0.5f;

	void Init() override;
	void GenerateMeshlets();
};
