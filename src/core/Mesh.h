#pragma once
#include "BaseMesh.h"


class Mesh : public BaseMesh
{
public:

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);

protected:
	void Init() override;
};