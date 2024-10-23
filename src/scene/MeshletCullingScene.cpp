#include "MeshletCullingScene.h"

MeshletCullingScene::MeshletCullingScene()
{
	std::cout << "Base Dir: " << basePath << std::endl;

	grid = std::make_unique<Grid>();

	camera = std::make_shared<Camera>();
	topCamera = std::make_shared<Camera>();

	frustumRenderer = std::make_unique<FrustumRenderer>();

	std::string vertPath = basePath + "shaders\\meshletCullingScene.vert";
	std::string fragPath = basePath + "shaders\\meshletCullingScene.frag";
	shader = std::make_shared<Shader>(vertPath, fragPath);

	// Load model
	modelLoader = std::make_unique<ModelLoader>();
	std::string modelPath = basePath + "models\\bunny.obj";
	std::vector<std::shared_ptr<Mesh>> meshes = modelLoader->LoadModel(modelPath);
	if (meshes.empty())
	{
		std::cerr << "Failed to load model" << std::endl;
		return;
	}
	mesh = std::make_shared<MeshletMesh>(meshes[0]);
	mesh->RemapIndicesForMeshlets();

	renderObject = std::make_unique<RenderObject<MeshletMesh>>(mesh, shader);

	// Create view projection buffer
	viewProjectionMatrices.push_back(camera->GetViewProjectionMatrix());
	viewProjectionMatrices.push_back(topCamera->GetViewProjectionMatrix());
	viewProjectionBuffer = std::make_shared<GPUBuffer<glm::mat4>>(viewProjectionMatrices.size(), viewProjectionBindingPoint, viewProjectionMatrices, BufferType::UBO, GL_DYNAMIC_DRAW); // UBO, binding = 1
	renderObject->gpuResource.AddBuffer(viewProjectionBuffer);

	// Create bounding spheres buffer
	std::vector<BoundingSphere> bounds;
	mesh->GenetateMeshletBounds(bounds);
	boundingSpheresBuffer = std::make_shared<GPUBuffer<BoundingSphere>>(bounds.size(), boundingSpheresBindingPoint, bounds, BufferType::SSBO, GL_STATIC_READ); // SSBO, binding = 0

	// Create frustum planes buffer
	frustumPlanes.push_back(camera->GetFrustumPlanes());
	frustumPlanesBuffer = std::make_shared<GPUBuffer<FrustumPlanes>>(1, frustumPlanesBindingPoint, frustumPlanes, BufferType::UBO, GL_DYNAMIC_DRAW); // UBO, binding = 0

	// Create multi draw indirect command buffer
	multiDrawIndirectCommand = std::make_shared<MultiDrawIndirectCommand>(mesh->GenerateMultiDrawIndirectCommand());
	multiDrawIndirectCommand->SetBindingPointForCompute(mdiCommandsBindingPoint); // SSBO, binding = 1

	// Create compute
	std::string computePath = basePath + "shaders\\meshletCullingScene.comp";
	cullingCompute = std::make_shared<Compute>(computePath);
	cullingCompute->gpuResource.AddBuffer(boundingSpheresBuffer);

	// Get work group size
	workGroupSize = cullingCompute->GetWorkGroupSize();

	// Set meshlet count
	meshletCount = mesh->meshletCount;
	cullingCompute->gpuResource.SetInt("meshletCount", meshletCount);
}

MeshletCullingScene::~MeshletCullingScene()
{
}

void MeshletCullingScene::Start()
{
	// Register camera
	InputManager::RegisterCamera(*camera);

	// Setup camera
	camera->SetPosition(0.0f, 1.5f, 3.0);
	camera->Rotate(0.0f, -10.0f);
	camera->SetNearPlane(0.3f);
	camera->SetFarPlane(20.0f);
	camera->SetFoV(30.0f);

	topCamera->SetPosition(6.0f, 6.0f, 1.0);
	topCamera->Rotate(-80.0f, -45.0f);

	viewProjectionMatrices[0] = camera->GetViewProjectionMatrix();
	viewProjectionMatrices[1] = topCamera->GetViewProjectionMatrix();
	viewProjectionBuffer->Update(viewProjectionMatrices);

	renderObject->transform.UniformScaling(0.2f);
	renderObject->transform.Rotate(0.0f, 0.0f, 35.0f);
	renderObject->transform.Translate(1.0f, 0.0f, 0.0f);

	glm::vec3 scale = renderObject->transform.Scale();
	float maxScale = std::max(scale.x, std::max(scale.y, scale.z));
	cullingCompute->gpuResource.SetFloat("scale", maxScale);
}

void MeshletCullingScene::Update(float deltaTime)
{
	// Culling
	cullingCompute->gpuResource.SetMat4("model", renderObject->transform.GetModelMatrix());
	cullingCompute->gpuResource.SetMat4("invTransModel", renderObject->transform.GetInvTransModelMatrix());
	cullingCompute->gpuResource.SetMat4("view", camera->GetViewMatrix());
	cullingCompute->gpuResource.SetVec3("cameraPosition", camera->GetPosition());
	frustumPlanes[0] = camera->GetFrustumPlanes();
	frustumPlanesBuffer->Update(frustumPlanes);
	int numGroupX = (meshletCount + workGroupSize[0] - 1) / workGroupSize[0];
	cullingCompute->DispatchForDrawIndirect(numGroupX, 1, 1, *multiDrawIndirectCommand);

	// Barrier
	glMemoryBarrier(GL_COMMAND_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	// Draw per view
	camera->SetViewport();
	grid->Draw(*camera);
	topCamera->SetViewport();
	grid->Draw(*topCamera);
	frustumRenderer->UpdateMesh(*camera);
	frustumRenderer->Draw(*topCamera);

	// Set viewport
	glViewportArrayv(0, 2, &viewportArray[0][0]);

	// Draw
	viewProjectionMatrices[0] = camera->GetViewProjectionMatrix();
	viewProjectionBuffer->Update(0, viewProjectionMatrices[0]);
	renderObject->MultiDrawIndirect(*camera, *multiDrawIndirectCommand);
}

void MeshletCullingScene::OnWindowResize(int width, int height)
{
	float halfWidth = width / 2.0f;
	float fHeight = static_cast<float>(height);
	float aspectRatio = halfWidth / fHeight;
	camera->SetAspectRatio(aspectRatio);
	camera->SetViewportParams(0, 0, halfWidth, fHeight);
	topCamera->SetAspectRatio(aspectRatio);
	topCamera->SetViewportParams(halfWidth, 0, halfWidth, fHeight);

	viewportArray[0][2] = halfWidth;
	viewportArray[0][3] = fHeight;
	viewportArray[1][0] = halfWidth;
	viewportArray[1][2] = halfWidth;
	viewportArray[1][3] = fHeight;
}