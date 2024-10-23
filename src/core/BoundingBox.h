#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "BaseMesh.h"

struct AABB
{
	glm::vec4 min;
	glm::vec4 max;
};

class BoundingBox
{
public:
	BoundingBox(const BaseMesh& mesh);
	BoundingBox(const std::shared_ptr<BaseMesh>& mesh);
	AABB GetAABB() const;

private:
	AABB aabb;
	AABB CalculateAABB(const BaseMesh& mesh);
};