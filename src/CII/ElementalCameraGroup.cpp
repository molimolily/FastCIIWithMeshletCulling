#include "ElementalCameraGroup.h"
#include "Eye.h"

ElementalCameraGroup::ElementalCameraGroup(int idx, int idy, std::shared_ptr<CIISettings> settings)
{
	groupID = glm::ivec2(idx, idy);
	this->settings = settings;
	for (int i = 0; i < settings->MaxGroupSizeX; i++)
	{
		int globalIdx = groupID.x * settings->MaxGroupSizeX + i;
		for (int j = 0; j < settings->MaxGroupSizeY; j++)
		{
			int globalIdy = groupID.y * settings->MaxGroupSizeY + j;
			cameras.push_back(ElementalCamera(globalIdx, globalIdy, settings));
		}
	}
	SetGroupFrustum();
}

ElementalCameraGroup::~ElementalCameraGroup()
{
}

std::vector<glm::mat4> ElementalCameraGroup::GetViewMatrices()
{
	viewMatrices.clear();
	for (int i = 0; i < cameras.size(); i++)
	{
		viewMatrices.push_back(cameras[i].camera.GetViewMatrix());
	}
	return viewMatrices;
}

std::vector<glm::mat4> ElementalCameraGroup::GetProjectionMatrices()
{
	projectionMatrices.clear();
	for (int i = 0; i < cameras.size(); i++)
	{
		projectionMatrices.push_back(cameras[i].camera.GetProjectionMatrix());
	}
	return projectionMatrices;
}

std::vector<glm::mat4> ElementalCameraGroup::GetViewProjectionMatrices()
{
	viewProjectionMatrices.clear();
	for (int i = 0; i < cameras.size(); i++)
	{
		viewProjectionMatrices.push_back(cameras[i].camera.GetViewProjectionMatrix());
	}
	return viewProjectionMatrices;
}

std::vector<Viewport> ElementalCameraGroup::GetViewports()
{
	viewports.clear();
	for (int i = 0; i < cameras.size(); i++)
	{
		ElementalCamera camera = cameras[i];
		camera.SetViewportParams();
		viewports.push_back(camera.camera.GetViewportParams());
	}
	return viewports;
}

void ElementalCameraGroup::SetGroupFrustum()
{
	glm::vec3 eye = Eye::pos;
	float gap = settings->gap;
	glm::vec2 lensSize = settings->lensSize;
	glm::ivec2 lensCount = settings->GetLensCount();
	int maxGroupSize = std::max(settings->MaxGroupSizeX, settings->MaxGroupSizeY);
	float t = (maxGroupSize - 1) * gap / (maxGroupSize * gap + eye.z);
	float apexX = (1.0f - t) * (maxGroupSize * groupID.x - (lensCount.x - maxGroupSize) / 2.0f) * lensSize.x + t * eye.x;
	float apexY = (1.0f - t) * (maxGroupSize * groupID.y - (lensCount.y - maxGroupSize) / 2.0f) * lensSize.y + t * eye.y;
	float apexZ = t * eye.z;
	groupCamera.SetPosition(apexX, apexY, apexZ);

	// right-top element
	int rtID = settings->MaxGroupSizeX * settings->MaxGroupSizeY - 1;
	ElementalCamera& rtCamera = cameras[rtID];
	Rect rtRect = rtCamera.camera.GetFrustumRect();

	// leftr-bottom element
	int lbID = 0;
	ElementalCamera& lbCamera = cameras[lbID];
	Rect lbRect = lbCamera.camera.GetFrustumRect();

	float left = lbRect.left + lbCamera.lensPos.x - groupCamera.GetPosition().x;
	float right = rtRect.right + rtCamera.lensPos.x - groupCamera.GetPosition().x;
	float bottom = lbRect.bottom + lbCamera.lensPos.y - groupCamera.GetPosition().y;
	float top = rtRect.top + rtCamera.lensPos.y - groupCamera.GetPosition().y;
	float near = settings->near + groupCamera.GetPosition().z;
	float far = settings->far + groupCamera.GetPosition().z;

	groupCamera.SetFrustumRect(left, right, bottom, top);
	groupCamera.SetNearPlane(near);
	groupCamera.SetFarPlane(far);
}