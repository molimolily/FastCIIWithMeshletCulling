#include "MultiDrawIndirectCommand.h"

MultiDrawIndirectCommand::MultiDrawIndirectCommand(const std::vector<DrawElementsIndirectCommand>& commands) : drawCount(commands.size())
{
	SetupBuffer(commands);
}

MultiDrawIndirectCommand::MultiDrawIndirectCommand(MultiDrawIndirectCommand&& other) noexcept
	: buffer(other.buffer), drawCount(other.drawCount), bindingPoint(other.bindingPoint)
{
	other.buffer = 0;
}

MultiDrawIndirectCommand& MultiDrawIndirectCommand::operator=(MultiDrawIndirectCommand&& other) noexcept
{
	if (this != &other)
	{
		if(buffer != 0)
			glDeleteBuffers(1, &buffer);
		buffer = other.buffer;
		drawCount = other.drawCount;
		bindingPoint = other.bindingPoint;
		other.buffer = 0;
	}
	return *this;
}

MultiDrawIndirectCommand::~MultiDrawIndirectCommand()
{
	glDeleteBuffers(1, &buffer);
}

void MultiDrawIndirectCommand::SetupBuffer(const std::vector<DrawElementsIndirectCommand>& commands)
{
	// Create buffer
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, commands.size() * sizeof(DrawElementsIndirectCommand), commands.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}

void MultiDrawIndirectCommand::Bind() const
{
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
}

void MultiDrawIndirectCommand::BindForCompute() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, buffer);
}

void MultiDrawIndirectCommand::SetBindingPointForCompute(const GLuint bindingPoint)
{
	this->bindingPoint = bindingPoint;
}

GLuint MultiDrawIndirectCommand::GetBuffer() const
{
	return buffer;
}

GLsizei MultiDrawIndirectCommand::GetDrawCount() const
{
	return drawCount;
}