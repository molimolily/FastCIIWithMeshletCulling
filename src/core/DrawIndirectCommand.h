#pragma once
#include <glad/glad.h>
#include <vector>
#include "IDrawIndirectCommand.h"

class DrawIndirectCommand : public IDrawIndirectCommand
{
public:
	DrawIndirectCommand(const DrawElementsIndirectCommand& command);
	DrawIndirectCommand(const unsigned int vertexCount, const unsigned int instanceCount, const unsigned int firstIndex, const unsigned int baseVertex, const unsigned int baseInstance);
	~DrawIndirectCommand();
	void Bind() const;
	void BindForCompute() const;
	void SetBindingPointForCompute(const GLuint bindingPoint);
	GLuint GetBuffer() const;
	void DebugPrint() const;
	void ResetInstanceCount(const unsigned int instanceCount);

private:
	GLuint buffer;
	GLuint bindingPoint = 0;
	void SetupBuffer(const DrawElementsIndirectCommand& command);
};