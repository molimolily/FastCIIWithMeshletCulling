#include "Compute.h"

Compute::Compute(std::shared_ptr<Shader> shader) : gpuResource(shader)
{
}

Compute::Compute(const std::string& computePath) : gpuResource(std::make_shared<Shader>(computePath))
{
}

void Compute::Dispatch(const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ)
{
	gpuResource.Bind();
	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
}

void Compute::DispatchForDrawIndirect(const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ, const IDrawIndirectCommand& indirectCommand)
{
	indirectCommand.BindForCompute();
	Dispatch(numGroupsX, numGroupsY, numGroupsZ);
}


std::array<GLint, 3> Compute::GetWorkGroupSize() const
{
	std::array<GLint, 3> size = { 1, 1, 1 };
	glGetProgramiv(gpuResource.GetShaderProgram(), GL_COMPUTE_WORK_GROUP_SIZE, size.data());
	return size;
}