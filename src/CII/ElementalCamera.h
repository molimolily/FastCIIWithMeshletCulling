#pragma once
#include <memory>
#include "CIISettings.h"
#include "Camera.h"

class ElementalCamera
{
public:
	glm::ivec2 id;
	glm::vec3 lensPos; // CII���W�ɂ�����ʒu
	Camera camera;
	std::shared_ptr<CIISettings> settings;

	ElementalCamera(int i, int j, std::shared_ptr<CIISettings> settings);
	~ElementalCamera();

	void SetPosition();
	void SetViewportParams();
	void SetFrustum();
};