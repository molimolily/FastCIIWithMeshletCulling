#pragma once
#include <glm/glm.hpp>

#define DEFAULT_EYE_POS glm::vec3(0.0f, 0.0f, 400.0f)

class Eye
{
private:
	Eye();
public:
	static inline glm::vec3 pos = DEFAULT_EYE_POS;
};