#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

enum class ShaderType
{
    VERTEX,
    FRAGMENT,
    COMPUTE,
    MESH,
    TASK
};

class Shader
{
public:
    Shader(const std::string& path, const ShaderType type = ShaderType::COMPUTE);
    Shader(const std::string& path1, const std::string& path2, const ShaderType type1 = ShaderType::VERTEX, const ShaderType type2 = ShaderType::FRAGMENT);
    Shader(const std::string& path1, const std::string& path2, const std::string& path3, const ShaderType type1 = ShaderType::TASK, const ShaderType type2 = ShaderType::MESH, const ShaderType type3 = ShaderType::FRAGMENT);

    ~Shader();

    void Use();

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w) const;
    void SetMat2(const std::string& name, const glm::mat2& mat) const;
    void SetMat3(const std::string& name, const glm::mat3& mat) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

    GLuint GetProgram() const;
private:
    GLuint program;
    std::vector<GLenum> types;
    void Compile(const std::string& path, GLenum shaderType);
    void Link();
    GLenum ShaderTypeToGLenum(ShaderType type);
};