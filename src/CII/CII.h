#pragma once
#include <memory>
#include "CIISettings.h"
#include "ElementalCameraGroup.h"

class CII
{
public:
	CII(const CIISettings& settings);
	~CII();

	std::shared_ptr<CIISettings> settings;
	std::vector<ElementalCameraGroup> cameraGroups;
};