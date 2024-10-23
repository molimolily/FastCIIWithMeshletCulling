#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
	: BaseMesh(vertices, indices)
{
	Init();
}

void Mesh::Init()
{
	OptimizeMesh();
	SetupMesh();
}