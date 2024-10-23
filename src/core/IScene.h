#pragma once
#include <iostream>
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshletMesh.h"
#include "RenderObject.h"
#include "GPUBuffer.h"
#include "Compute.h"
#include "DrawIndirectCommand.h"
#include "MultiDrawIndirectCommand.h"
#include "ModelLoader.h"
#include "BoundingBox.h"
#include "Grid.h"
#include "InputManager.h"
#include "FrustumRenderer.h"

extern std::string basePath;

class IScene
{
public:
	virtual ~IScene() {};
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;

	virtual void OnWindowResize(int width, int height) = 0;
};
