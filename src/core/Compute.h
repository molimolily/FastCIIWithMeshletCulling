#pragma once
#include <glad/glad.h>
#include <vector>
#include <array>
#include <memory>
#include <functional>
#include <queue>
#include "Shader.h"
#include "IDrawIndirectCommand.h"
#include "GPUResourceManager.h"


class Compute
{
public:
	GPUResourceManager gpuResource;

    Compute(std::shared_ptr<Shader> shader);
	Compute(const std::string& computePath);

	void Dispatch(const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ);
	void DispatchForDrawIndirect(const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ, const IDrawIndirectCommand& indirectCommand);

    std::array<int, 3> GetWorkGroupSize() const;

};
