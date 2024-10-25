#include "ElementalCamera.h"
#include "CIISettings.h"
#include "Eye.h"

#include <iostream>

ElementalCamera::ElementalCamera(int i, int j, std::shared_ptr<CIISettings> settings)
{
	id.x = i;
	id.y = j;

	this->settings = settings;

	camera.SetNearPlane(settings->near);
	camera.SetFarPlane(settings->far);
	SetPosition();
	SetViewportParams();
	SetFrustum();
}

ElementalCamera::~ElementalCamera()
{
}

void ElementalCamera::SetPosition()
{
	glm::ivec2 lensCount = settings->GetLensCount();
	float x = (id.x - (lensCount.x - 1) / 2.0f) * settings->lensSize.x;
	float y = (id.y - (lensCount.y - 1) / 2.0f) * settings->lensSize.y;
	lensPos = glm::vec3(x, y, 0);
	camera.SetPosition(lensPos);
}

void ElementalCamera::SetViewportParams()
{
	// �v�f�摜�̈ʒu���v�Z
	float t = -settings->gap / Eye::pos.z;
	glm::vec3 imagePosRS = (1.0f - t) * lensPos + t * Eye::pos; // real-world space

	// �v�f�摜�̈ʒu���s�N�Z����Ԃɕϊ�
	glm::vec2 scale = glm::vec2((float)settings->panelResolution.x / settings->panelSize.x, (float)settings->panelResolution.y / settings->panelSize.y);
	glm::vec2 imagePosPS = glm::vec2(imagePosRS.x * scale.x + settings->panelResolution.x / 2.0f, imagePosRS.y * scale.y + settings->panelResolution.y / 2.0f); // pixel space

	// �v�f�摜�̃T�C�Y���v�Z
	float magnification = 1.0f + settings->gap / Eye::pos.z;
	glm::ivec2 imageSize = glm::ivec2(std::ceill(settings->lensSize.x * scale.x * magnification), std::ceill(settings->lensSize.y * scale.y * magnification));

	// �v�f�摜�̈ʒu�ƃT�C�Y��ݒ�
	glm::vec2 offset = settings->viewportPosOffset;
	camera.SetViewportParams(imagePosPS.x - imageSize.x / 2.0f + offset.x, imagePosPS.y - imageSize.y / 2.0f + offset.y, imageSize.x, imageSize.y);
}

void ElementalCamera::SetFrustum()
{
	glm::vec3 eye = Eye::pos;
	glm::ivec2 lensCount = settings->GetLensCount();
	float gap = settings->gap;
	float near = settings->near;

	float right = ((id.x - lensCount.x / 2.0f + 1.0f + eye.z / gap * 0.5f) * settings->lensSize.x - eye.x) * near / eye.z;
	float left = ((id.x - lensCount.x / 2.0f - eye.z / gap * 0.5f) * settings->lensSize.x - eye.x) * near / eye.z;
	float top = ((id.y - lensCount.y / 2.0f + 1.0f + eye.z / gap * 0.5f) * settings->lensSize.y - eye.y) * near / eye.z;
	float bottom = ((id.y - lensCount.y / 2.0f - eye.z / gap * 0.5f) * settings->lensSize.y - eye.y) * near / eye.z;
	Rect frustumRect(left, right, bottom, top);

	camera.SetFrustumRect(frustumRect);
}