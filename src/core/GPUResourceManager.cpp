#include "GPUResourceManager.h"

GPUResourceManager::GPUResourceManager(std::shared_ptr<Shader> shader)
	: shader(shader)
{
}

void GPUResourceManager::Bind()
{
	shader->Use();

	while (!deferredCalls.empty())
	{
		deferredCalls.front()();
		deferredCalls.pop();
	}

	for (unsigned int i = 0; i < buffers.size(); i++)
	{
		buffers[i]->Bind();
	}
}

void GPUResourceManager::AddBuffer(const std::shared_ptr<IGPUBuffer>& buffer)
{
	buffers.push_back(buffer);
}

void GPUResourceManager::ClearBuffers()
{
	buffers.clear();
}

void GPUResourceManager::SetBool(const std::string& name, bool value)
{
	deferredCalls.push([=]() { shader->SetBool(name, value); });
}


void GPUResourceManager::SetInt(const std::string& name, int value)
{
	deferredCalls.push([=]() { shader->SetInt(name, value); });
}

void GPUResourceManager::SetFloat(const std::string& name, float value)
{
	deferredCalls.push([=]() { shader->SetFloat(name, value); });
}

void GPUResourceManager::SetVec2(const std::string& name, const glm::vec2& value)
{
	deferredCalls.push([=]() { shader->SetVec2(name, value); });
}

void GPUResourceManager::SetVec2(const std::string& name, float x, float y)
{
	deferredCalls.push([=]() { shader->SetVec2(name, x, y); });
}

void GPUResourceManager::SetVec3(const std::string& name, const glm::vec3& value)
{
	deferredCalls.push([=]() { shader->SetVec3(name, value); });
}

void GPUResourceManager::SetVec3(const std::string& name, float x, float y, float z)
{
	deferredCalls.push([=]() { shader->SetVec3(name, x, y, z); });
}

void GPUResourceManager::SetVec4(const std::string& name, const glm::vec4& value)
{
	deferredCalls.push([=]() { shader->SetVec4(name, value); });
}

void GPUResourceManager::SetVec4(const std::string& name, float x, float y, float z, float w)
{
	deferredCalls.push([=]() { shader->SetVec4(name, x, y, z, w); });
}

void GPUResourceManager::SetMat2(const std::string& name, const glm::mat2& mat)
{
	deferredCalls.push([=]() { shader->SetMat2(name, mat); });
}

void GPUResourceManager::SetMat3(const std::string& name, const glm::mat3& mat)
{
	deferredCalls.push([=]() { shader->SetMat3(name, mat); });
}

void GPUResourceManager::SetMat4(const std::string& name, const glm::mat4& mat)
{
	deferredCalls.push([=]() { shader->SetMat4(name, mat); });
}

GLuint GPUResourceManager::GetShaderProgram() const
{
	return shader->GetProgram();
}