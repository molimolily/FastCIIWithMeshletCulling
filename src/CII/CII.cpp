#include "CII.h"

CII::CII(const CIISettings& settings)
{
	this->settings = std::make_shared<CIISettings>(settings);
	glm::ivec2 lensGroupCount = this->settings->GetLensGroupCount();
	for (int i = 0; i < lensGroupCount.x; i++)
	{
		for (int j = 0; j < lensGroupCount.y; j++)
		{
			cameraGroups.push_back(ElementalCameraGroup(i, j, this->settings));
		}
	}
}

CII::~CII()
{
}