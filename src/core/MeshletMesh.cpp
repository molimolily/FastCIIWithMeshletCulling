#include "MeshletMesh.h"
#include <meshoptimizer.h>
#include <iostream>

MeshletMesh::MeshletMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
	: BaseMesh(vertices, indices)
{
	Init();
}

MeshletMesh::MeshletMesh(const std::shared_ptr<Mesh>& mesh)
	: BaseMesh(mesh->GetVertices(), mesh->GetIndices())
{
	Init();
}

void MeshletMesh::Init()
{
	OptimizeMesh();
	GenerateMeshlets();
	SetupMesh();
}

MultiDrawIndirectCommand MeshletMesh::GenerateMultiDrawIndirectCommand(GLuint instanceCount) const
{
	std::vector<DrawElementsIndirectCommand> commands;
	for (int i = 0; i < meshletCount; i++)
	{
		GLuint count = meshlets[i].triangle_count * 3;
		GLuint firstIndex = indexOffsets[i];
		GLuint baseVertex = 0;
		GLuint baseInstance = 0;
		commands.push_back({ count, instanceCount, firstIndex, baseVertex, baseInstance });
	}
	return MultiDrawIndirectCommand(commands);
}

void MeshletMesh::GenerateMeshlets()
{
	size_t maxMeshlets = meshopt_buildMeshletsBound(indices.size(), maxVerticesPerMeshlet, maxPrimitivesPerMeshlet);
	meshlets.resize(maxMeshlets);
	meshletVertices.resize(maxMeshlets * maxVerticesPerMeshlet);
	meshletPrimitives.resize(maxMeshlets * maxPrimitivesPerMeshlet);

	meshletCount = meshopt_buildMeshlets(meshlets.data(), meshletVertices.data(), meshletPrimitives.data(),
		indices.data(), indices.size(), &vertices[0].position.x, vertices.size(), sizeof(Vertex),
		maxVerticesPerMeshlet, maxPrimitivesPerMeshlet, coneWeight);
	std::cout << "Meshlet count: " << meshletCount << std::endl;

	const meshopt_Meshlet& last = meshlets[meshletCount - 1];
	meshletVertices.resize(last.vertex_offset + last.vertex_count);
	meshletPrimitives.resize(last.triangle_offset + ((last.triangle_count * 3 + 3) & ~3));
	meshlets.resize(meshletCount);
	indexOffsets.resize(meshletCount);
}

void MeshletMesh::RemapIndicesForMeshlets()
{
	std::vector<GLuint> newIndices;
	GLuint offset = 0;
	for (int i = 0; i < meshletCount; i++)
	{
		indexOffsets[i] = offset;
		unsigned int vertexCount = meshlets[i].vertex_count;
		unsigned int vertexOffset = meshlets[i].vertex_offset;
		unsigned int triangleCount = meshlets[i].triangle_count;
		unsigned int triangleOffset = meshlets[i].triangle_offset;
		for (int j = 0; j < triangleCount; j++)
		{
			unsigned int i0 = static_cast<unsigned int>(meshletPrimitives[triangleOffset + 3 * j]);
			unsigned int i1 = static_cast<unsigned int>(meshletPrimitives[triangleOffset + 3 * j + 1]);
			unsigned int i2 = static_cast<unsigned int>(meshletPrimitives[triangleOffset + 3 * j + 2]);
			unsigned int v0 = meshletVertices[vertexOffset + i0];
			unsigned int v1 = meshletVertices[vertexOffset + i1];
			unsigned int v2 = meshletVertices[vertexOffset + i2];

			newIndices.push_back(v0);
			newIndices.push_back(v1);
			newIndices.push_back(v2);

			offset += 3;
		}
	}
	indices = std::move(newIndices);

	UploadIndices();
}

void MeshletMesh::GenetateMeshletBounds(std::vector<BoundingSphere>& bounds)
{
	bounds.resize(meshletCount);
	for (int i = 0; i < meshletCount; i++)
	{
		meshopt_Meshlet& meshlet = meshlets[i];
		meshopt_Bounds meshletBound = meshopt_computeMeshletBounds(&meshletVertices[meshlet.vertex_offset], 
														&meshletPrimitives[meshlet.triangle_offset],
														meshlet.triangle_count,
														&vertices[0].position.x,
														vertices.size(),			
														sizeof(Vertex)
														);	
		bounds[i].center = glm::vec3(meshletBound.center[0], meshletBound.center[1], meshletBound.center[2]);
		bounds[i].radius = meshletBound.radius;
		bounds[i].coneAxis = glm::vec3(meshletBound.cone_axis[0], meshletBound.cone_axis[1], meshletBound.cone_axis[2]);
		bounds[i].coneCutOff = meshletBound.cone_cutoff;
	}
}
