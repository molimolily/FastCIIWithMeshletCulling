#pragma once
#include <glad/glad.h>
#include <vector>
#include "IDrawIndirectCommand.h"

class MultiDrawIndirectCommand : public IDrawIndirectCommand
{
public:
	MultiDrawIndirectCommand(const std::vector<DrawElementsIndirectCommand>& commands);

	// Move constructor
	MultiDrawIndirectCommand(MultiDrawIndirectCommand&& other) noexcept;

	// Move assignment operator
	MultiDrawIndirectCommand& operator=(MultiDrawIndirectCommand&& other) noexcept;

	~MultiDrawIndirectCommand();
	void Bind() const;
	void BindForCompute() const;
	void SetBindingPointForCompute(const GLuint bindingPoint);
	GLuint GetBuffer() const;
	GLsizei GetDrawCount() const;

private:
	GLuint buffer;
	GLuint bindingPoint = 0;
	GLsizei drawCount;
	void SetupBuffer(const std::vector<DrawElementsIndirectCommand>& command);
};
