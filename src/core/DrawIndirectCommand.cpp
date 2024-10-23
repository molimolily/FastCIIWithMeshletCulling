#include "DrawIndirectCommand.h"
#include <iostream>

DrawIndirectCommand::DrawIndirectCommand(const DrawElementsIndirectCommand& command)
{
	SetupBuffer(command);
}

DrawIndirectCommand::DrawIndirectCommand(const unsigned int vertexCount, const unsigned int instanceCount, const unsigned int firstIndex, const unsigned int baseVertex, const unsigned int baseInstance)
{
	DrawElementsIndirectCommand cmd = { vertexCount, instanceCount, firstIndex, baseVertex, baseInstance };
	SetupBuffer(cmd);
}

DrawIndirectCommand::~DrawIndirectCommand()
{
	glDeleteBuffers(1, &buffer);
}

void DrawIndirectCommand::Bind() const
{
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
}

void DrawIndirectCommand::BindForCompute() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, buffer);
}

void DrawIndirectCommand::SetBindingPointForCompute(const GLuint bindingPoint)
{
	this->bindingPoint = bindingPoint;
}

GLuint DrawIndirectCommand::GetBuffer() const
{
	return buffer;
}

void DrawIndirectCommand::SetupBuffer(const DrawElementsIndirectCommand& command)
{
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand), &command, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}



void DrawIndirectCommand::ResetInstanceCount(const unsigned int instanceCount)
{
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
	DrawElementsIndirectCommand* cmd = (DrawElementsIndirectCommand*)glMapBuffer(GL_DRAW_INDIRECT_BUFFER, GL_WRITE_ONLY);
	cmd->instanceCount = instanceCount;
	glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}

void DrawIndirectCommand::DebugPrint() const
{
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
	DrawElementsIndirectCommand* cmd = (DrawElementsIndirectCommand*)glMapBuffer(GL_DRAW_INDIRECT_BUFFER, GL_READ_ONLY);
	// std::cout << "vertexCount: " << cmd->vertexCount << std::endl;
	std::cout << "instanceCount: " << cmd->instanceCount << std::endl;
	// std::cout << "firstIndex: " << cmd->firstIndex << std::endl;
	// std::cout << "baseVertex: " << cmd->baseVertex << std::endl;
	// std::cout << "baseInstance: " << cmd->baseInstance << std::endl;
	glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}