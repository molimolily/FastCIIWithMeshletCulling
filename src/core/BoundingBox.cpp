#include "BoundingBox.h"
#include <limits>

BoundingBox::BoundingBox(const BaseMesh& mesh)
	: aabb(CalculateAABB(mesh))
{
}

BoundingBox::BoundingBox(const std::shared_ptr<BaseMesh>& mesh)
	: aabb(CalculateAABB(*mesh))
{
}

AABB BoundingBox::CalculateAABB(const BaseMesh& mesh)
{
	glm::vec4 min(std::numeric_limits<float>::max());
	glm::vec4 max(std::numeric_limits<float>::min());
	
	min.w = 1.0f;
	max.w = 1.0f;

	for (const auto& vertex : mesh.GetVertices())
	{
		min.x = std::min(min.x, vertex.position.x);
		min.y = std::min(min.y, vertex.position.y);
		min.z = std::min(min.z, vertex.position.z);
		
		max.x = std::max(max.x, vertex.position.x);
		max.y = std::max(max.y, vertex.position.y);
		max.z = std::max(max.z, vertex.position.z);
	}

	return AABB{ min, max };
}

AABB BoundingBox::GetAABB() const
{
	return aabb;
}