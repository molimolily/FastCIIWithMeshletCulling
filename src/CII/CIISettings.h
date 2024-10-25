#pragma once
#include <glm/glm.hpp>

class CIISettings
{
private:
    glm::ivec2 lensCount = glm::ivec2(4, 4);
    glm::ivec2 lensGroupCount = glm::ivec2(1, 1);

public:
    CIISettings(glm::ivec2 lensGroupCount = glm::ivec2(1,1))
    {
        SetGroupCount(lensGroupCount.x, lensGroupCount.y);
    }
    const int MaxGroupSizeX = 4;
    const int MaxGroupSizeY = 4;

    glm::vec2 panelSize = glm::vec2(15.6f, 27.7f);
    glm::ivec2 panelResolution = glm::ivec2(2160, 3840);

    glm::vec2 lensSize = glm::vec2(0.48f, 0.48f);
    float focalLength = 1.5f;

    float gap = 1.3f;
    float near = 0.3f;
    float far = 20.0f;

    glm::vec2 viewportPosOffset = glm::vec2(0, 0);

    void SetGroupCount(int x, int y)
    {
       lensGroupCount = glm::ivec2(x, y);
       lensCount = glm::ivec2(x * MaxGroupSizeX, y * MaxGroupSizeY);
    }

    glm::ivec2 GetLensCount() { return lensCount; }
    glm::ivec2 GetLensGroupCount() { return lensGroupCount; }
};