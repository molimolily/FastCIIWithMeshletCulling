#pragma once
#include <glad/glad.h>

struct DrawElementsIndirectCommand
{
	unsigned int vertexCount;
	unsigned int instanceCount;
	unsigned int firstIndex;
	unsigned int baseVertex;
	unsigned int baseInstance;
};

class IDrawIndirectCommand
{
public:
	virtual ~IDrawIndirectCommand() = default;
	virtual void Bind() const = 0;
	virtual void BindForCompute() const = 0;
	virtual void SetBindingPointForCompute(const GLuint bindingPoint) = 0;
};