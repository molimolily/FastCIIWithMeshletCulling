#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <memory>
#include <glm/gtx/string_cast.hpp>
#include "BaseMesh.h"
#include "MeshletMesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "DrawIndirectCommand.h"
#include "MultiDrawIndirectCommand.h"
#include "GPUResourceManager.h"

template <typename T>
concept TMesh = std::is_base_of<BaseMesh, T>::value;

template <TMesh T>
class RenderObject
{
public:
		std::shared_ptr<T> mesh;
		Transform transform;
		GPUResourceManager gpuResource;

		RenderObject(std::shared_ptr<T> mesh, std::shared_ptr<Shader> shader, Transform transform = Transform())
			: mesh(mesh), gpuResource(shader), transform(transform)
		{
		}

		void Draw()
		{
			gpuResource.Bind();
			mesh->Draw();
		}

		void Draw(Camera& camera)
		{
			gpuResource.SetMat4("model", transform.GetModelMatrix());
			gpuResource.SetMat4("view", camera.GetViewMatrix());
			gpuResource.SetMat4("projection", camera.GetProjectionMatrix());
			Draw();
		}

		void DrawInstanced(unsigned int instanceCount)
		{
			gpuResource.Bind();
			mesh->DrawInstanced(instanceCount);
		}

		void DrawInstanced(Camera& camera, unsigned int instanceCount)
		{
			gpuResource.SetMat4("model", transform.GetModelMatrix());
			gpuResource.SetMat4("view", camera.GetViewMatrix());
			gpuResource.SetMat4("projection", camera.GetProjectionMatrix());
			DrawInstanced(instanceCount);
		}

		void DrawIndirect(const DrawIndirectCommand& indirectCommand)
		{
			gpuResource.Bind();
			mesh->DrawIndirect(indirectCommand);
		}

		void DrawIndirect(Camera& camera, const DrawIndirectCommand& indirectCommand)
		{
			gpuResource.SetMat4("model", transform.GetModelMatrix());
			gpuResource.SetMat4("view", camera.GetViewMatrix());
			gpuResource.SetMat4("projection", camera.GetProjectionMatrix());
			DrawIndirect(indirectCommand);
		}

		void MultiDrawIndirect(const MultiDrawIndirectCommand& indirectCommand)
		{
			gpuResource.Bind();
			mesh->MultiDrawIndirect(indirectCommand);
		}

		void MultiDrawIndirect(Camera& camera, const MultiDrawIndirectCommand& indirectCommand)
		{
			gpuResource.SetMat4("model", transform.GetModelMatrix());
			gpuResource.SetMat4("view", camera.GetViewMatrix());
			gpuResource.SetMat4("projection", camera.GetProjectionMatrix());
			MultiDrawIndirect(indirectCommand);
		}

		template <typename U = T>
		std::enable_if_t<std::is_same<U, MeshletMesh>::value, void> DrawMeshTasks(unsigned int count, unsigned int first = 0)
		{
			gpuResource.SetInt("meshletCount", mesh->meshletCount);
			gpuResource.Bind();
			glDrawMeshTasksNV(first, count);
		}

		template <typename U = T>
		std::enable_if_t<std::is_same<U, MeshletMesh>::value, void> DrawMeshTasks(Camera& camera, unsigned int count, unsigned int first = 0)
		{
			gpuResource.SetMat4("model", transform.GetModelMatrix());
			gpuResource.SetMat4("invTransModel", transform.GetInvTransModelMatrix());
			gpuResource.SetMat4("view", camera.GetViewMatrix());
			gpuResource.SetMat4("projection", camera.GetProjectionMatrix());
			gpuResource.SetVec3("cameraPosition", camera.GetPosition());
			this->DrawMeshTasks(count, first);
		}
};
