#pragma once
#include "IScene.h"

class MeshletCullingScene : public IScene
{
public :
		MeshletCullingScene();
		~MeshletCullingScene();

		void Start() override;
		void Update(float deltaTime) override;

		void OnWindowResize(int width, int height) override;
private:
	std::unique_ptr<ModelLoader> modelLoader;
	std::unique_ptr<Grid> grid;
	std::unique_ptr<RenderObject<MeshletMesh>> renderObject;
	std::unique_ptr<FrustumRenderer> frustumRenderer;

	// resources
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Camera> topCamera;
	std::shared_ptr<MeshletMesh> mesh;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<GPUBuffer<glm::mat4>> viewProjectionBuffer;
	std::shared_ptr<GPUBuffer<BoundingSphere>> boundingSpheresBuffer;
	std::shared_ptr<GPUBuffer<FrustumPlanes>> frustumPlanesBuffer;
	std::shared_ptr<MultiDrawIndirectCommand> multiDrawIndirectCommand;
	std::shared_ptr<Compute> cullingCompute;
	std::array<GLint, 3> workGroupSize = { 0, 0, 0 };
	int meshletCount = 0;
	std::vector <glm::mat4> viewProjectionMatrices;
	std::vector<FrustumPlanes> frustumPlanes;
	GLfloat viewportArray[2][4] = { {0, 0, 800, 600}, {800, 0, 800, 600} };

	// settings
	GLuint boundingSpheresBindingPoint = 0; // SSBO binding point
	GLuint mdiCommandsBindingPoint = 1; // SSBO binding point
	GLuint frustumPlanesBindingPoint = 0; // UBO binding point
	GLuint viewProjectionBindingPoint = 1; // UBO binding point
};