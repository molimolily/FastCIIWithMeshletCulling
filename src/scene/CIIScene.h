#pragma once
#include "IScene.h"
#include "CII.h"

class CIIScene : public IScene
{
public :
	CIIScene();
	~CIIScene();

	void Start() override;
	void Update(float deltaTime) override;

	void OnWindowResize(int width, int height) override;

private:
	std::unique_ptr<ModelLoader> modelLoader;
	std::unique_ptr<RenderObject<MeshletMesh>> renderObject;
	std::unique_ptr<CII> cii;

	std::shared_ptr<MeshletMesh> mesh;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<GPUBuffer<glm::mat4>> viewProjectionBuffer;
	std::shared_ptr<GPUBuffer<BoundingSphere>> boundingSpheresBuffer;
	std::shared_ptr<GPUBuffer<FrustumPlanes>> frustumPlanesBuffer;
	std::shared_ptr<GPUBuffer<glm::vec3>> cornerCameraPositionBuffer;
	std::shared_ptr<MultiDrawIndirectCommand> mdiCmd;
	std::shared_ptr<Compute> cullingCompute;
	std::array<GLint, 3> workGroupSize = { 0, 0, 0 };
	std::vector<FrustumPlanes> frustumPlanes;
	std::vector<glm::vec3> cornerCameraPositions;
	int meshletCount = 0;
	int instanceCount = 16;

	GLuint mdiCommandsBindingPoint = 0; // SSBO binding point
	GLuint boundingSpheresBindingPoint = 1; // SSBO binding point
	GLuint viewProjectionBufferBindingPoint = 0; // UBO binding point
	GLuint frustumPlanesBindingPoint = 1; // UBO binding point
	GLuint cornerCameraPositionBindingPoint = 2; // UBO binding point

	float viewportShiftSpeed = 30.0f;

	void SetViewportArray(const std::vector<Viewport>& viewports);
};
