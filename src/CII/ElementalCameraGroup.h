#pragma once
#include "ElementalCamera.h"

class ElementalCameraGroup
{
public:
	ElementalCameraGroup(int idx, int idy, std::shared_ptr<CIISettings> settings);
	~ElementalCameraGroup();

	glm::ivec2 groupID;
	std::vector<ElementalCamera> cameras;
	Camera groupCamera;
	std::shared_ptr<CIISettings> settings;

	std::vector<glm::mat4> GetViewMatrices();
	std::vector<glm::mat4> GetProjectionMatrices();
	std::vector<glm::mat4> GetViewProjectionMatrices();
	std::vector<Viewport> GetViewports();

private:
	std::vector<glm::mat4> viewMatrices;
	std::vector<glm::mat4> projectionMatrices;
	std::vector<glm::mat4> viewProjectionMatrices;
	std::vector<Viewport> viewports;

	void SetGroupFrustum();
};
