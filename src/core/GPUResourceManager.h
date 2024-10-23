#pragma once
#include <glad/glad.h>
#include <vector>
#include <memory>
#include <functional>
#include <queue>
#include "Shader.h"
#include "IGPUBuffer.h"

class GPUResourceManager
{
public:
	GPUResourceManager(std::shared_ptr<Shader> shader);

    void Bind();

    void AddBuffer(const std::shared_ptr<IGPUBuffer>& buffer);
    void ClearBuffers();

    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec2(const std::string& name, const glm::vec2& value);
    void SetVec2(const std::string& name, float x, float y);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetVec3(const std::string& name, float x, float y, float z);
    void SetVec4(const std::string& name, const glm::vec4& value);
    void SetVec4(const std::string& name, float x, float y, float z, float w);
    void SetMat2(const std::string& name, const glm::mat2& mat);
    void SetMat3(const std::string& name, const glm::mat3& mat);
    void SetMat4(const std::string& name, const glm::mat4& mat);

    GLuint GetShaderProgram() const;

private:
    std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<IGPUBuffer>> buffers;
    std::queue<std::function<void()>> deferredCalls;
};
