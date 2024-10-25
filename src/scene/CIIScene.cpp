#include "CIIScene.h"
#include "CIISettings.h"
#include "Eye.h"

CIIScene::CIIScene()
{
	std::string vertePath = basePath + "shaders/CII.vert";
	std::string fragPath = basePath + "shaders/CII.frag";
	shader = std::make_shared<Shader>(vertePath, fragPath);

	// Load model
	modelLoader = std::make_unique<ModelLoader>();
	std::string modelPath = basePath + "models/bunny.obj";
	std::vector<std::shared_ptr<Mesh>> meshes = modelLoader->LoadModel(modelPath);
	

	if (meshes.empty())
	{
		std::cerr << "Failed to load model" << std::endl;
		return;
	}
	mesh = std::make_shared<MeshletMesh>(meshes[0]);
	mesh->RemapIndicesForMeshlets();

	renderObject = std::make_unique<RenderObject<MeshletMesh>>(mesh, shader);

	CIISettings settings(glm::ivec2(5, 10));
	settings.far = 100.0f;
	settings.viewportPosOffset = glm::vec2(-30.0f, -185.0f);
	cii = std::make_unique<CII>(settings);

	instanceCount = cii->settings->MaxGroupSizeX * cii->settings->MaxGroupSizeY;
	mdiCmd = std::make_shared<MultiDrawIndirectCommand>(mesh->GenerateMultiDrawIndirectCommand(instanceCount));

	// Create view projection buffer
	std::vector<glm::mat4> viewProjectionMatrices = cii->cameraGroups[0].GetViewProjectionMatrices();
	viewProjectionBuffer = std::make_shared<GPUBuffer<glm::mat4>>(viewProjectionMatrices.size(),viewProjectionBufferBindingPoint, viewProjectionMatrices, BufferType::UBO, GL_DYNAMIC_DRAW); // UBO
	renderObject->gpuResource.AddBuffer(viewProjectionBuffer);

	// Create bounding spheres buffer
	std::vector<BoundingSphere> bounds;
	mesh->GenetateMeshletBounds(bounds);
	boundingSpheresBuffer = std::make_shared<GPUBuffer<BoundingSphere>>(bounds.size(), boundingSpheresBindingPoint, bounds, BufferType::SSBO, GL_DYNAMIC_DRAW); // SSBO

	// Create frustum planes buffer
	frustumPlanes.push_back(cii->cameraGroups[0].groupCamera.GetFrustumPlanes());
	frustumPlanesBuffer = std::make_shared<GPUBuffer<FrustumPlanes>>(1, frustumPlanesBindingPoint, frustumPlanes, BufferType::UBO, GL_DYNAMIC_DRAW); // UBO

	// Create corner camera position buffer
	cornerCameraPositions.push_back(cii->cameraGroups[0].cameras[0].camera.GetPosition());
	cornerCameraPositions.push_back(cii->cameraGroups[0].cameras[cii->settings->MaxGroupSizeX - 1].camera.GetPosition());
	cornerCameraPositions.push_back(cii->cameraGroups[0].cameras[(cii->settings->MaxGroupSizeX - 1) * cii->settings->MaxGroupSizeY].camera.GetPosition());
	cornerCameraPositions.push_back(cii->cameraGroups[0].cameras[cii->settings->MaxGroupSizeX * cii->settings->MaxGroupSizeY - 1].camera.GetPosition());
	cornerCameraPositionBuffer = std::make_shared<GPUBuffer<glm::vec3>>(cornerCameraPositions.size(), cornerCameraPositionBindingPoint, cornerCameraPositions, BufferType::UBO, GL_DYNAMIC_DRAW); // UBO


	// Create compute
	std::string computePath = basePath + "shaders/CII.comp";
	cullingCompute = std::make_shared<Compute>(computePath);
	cullingCompute->gpuResource.AddBuffer(boundingSpheresBuffer);
	cullingCompute->gpuResource.AddBuffer(frustumPlanesBuffer);
	cullingCompute->gpuResource.AddBuffer(cornerCameraPositionBuffer);

	// Get work group size
	workGroupSize = cullingCompute->GetWorkGroupSize();

	meshletCount = mesh->meshletCount;
	cullingCompute->gpuResource.SetInt("meshletCount", meshletCount);
	cullingCompute->gpuResource.SetInt("instanceCount", instanceCount);
}

CIIScene::~CIIScene()
{
}

void CIIScene::Start()
{
	renderObject->transform.SetPosition(0.0f, -7.0f, -6.0f);
	renderObject->transform.UniformScaling(1.0f);

	Eye::pos.z = 200.0f;
	// Register camera
	for (auto& group : cii->cameraGroups)
	{
		InputManager::RegisterCamera(group.groupCamera);
		for (auto& camera : group.cameras)
		{
			InputManager::RegisterCamera(camera.camera);
		}
	}

}

void CIIScene::Update(float deltaTime)
{
	if(InputManager::IsKeyPressed(GLFW_KEY_UP))
	{
		cii->settings->viewportPosOffset.y += viewportShiftSpeed * deltaTime;
	}
	else if(InputManager::IsKeyPressed(GLFW_KEY_DOWN))
	{
		cii->settings->viewportPosOffset.y -= viewportShiftSpeed * deltaTime;
	}
	else if(InputManager::IsKeyPressed(GLFW_KEY_LEFT))
	{
		cii->settings->viewportPosOffset.x -= viewportShiftSpeed * deltaTime;
	}
	else if(InputManager::IsKeyPressed(GLFW_KEY_RIGHT))
	{
		cii->settings->viewportPosOffset.x += viewportShiftSpeed * deltaTime;
	}

	glm::vec3 scale = renderObject->transform.Scale();
	float maxScale = std::max(scale.x, std::max(scale.y, scale.z));
	cullingCompute->gpuResource.SetFloat("scale", maxScale);
	cullingCompute->gpuResource.SetMat4("model", renderObject->transform.GetModelMatrix());

	for(auto& group: cii->cameraGroups)
	{
		cullingCompute->gpuResource.SetMat4("view", group.groupCamera.GetViewMatrix());
		cullingCompute->gpuResource.SetVec3("cameraPosition", group.groupCamera.GetPosition());

		frustumPlanes[0] = group.groupCamera.GetFrustumPlanes();
		frustumPlanesBuffer->Update(frustumPlanes);

		cornerCameraPositions[0] = group.cameras[0].camera.GetPosition();
		cornerCameraPositions[1] = group.cameras[cii->settings->MaxGroupSizeX - 1].camera.GetPosition();
		cornerCameraPositions[2] = group.cameras[(cii->settings->MaxGroupSizeX - 1) * cii->settings->MaxGroupSizeY].camera.GetPosition();
		cornerCameraPositions[3] = group.cameras[cii->settings->MaxGroupSizeX * cii->settings->MaxGroupSizeY - 1].camera.GetPosition();
		cornerCameraPositionBuffer->Update(cornerCameraPositions);

		mdiCmd->SetBindingPointForCompute(mdiCommandsBindingPoint); // SSBO
		int numGroupX = (meshletCount + workGroupSize[0] - 1) / workGroupSize[0];
		cullingCompute->DispatchForDrawIndirect(numGroupX, 1, 1, *mdiCmd);

		// Barrier
		glMemoryBarrier(GL_COMMAND_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

		// Set viewport array
		SetViewportArray(group.GetViewports());

		// Draw
		renderObject->gpuResource.SetMat4("model", renderObject->transform.GetModelMatrix());
		viewProjectionBuffer->Update(group.GetViewProjectionMatrices());
		renderObject->MultiDrawIndirect(*mdiCmd);
	}
}

void CIIScene::OnWindowResize(int width, int height)
{
}

void CIIScene::SetViewportArray(const std::vector<Viewport>& viewports)
{
	glViewportArrayv(0, viewports.size(), (GLfloat*)viewports.data());
}