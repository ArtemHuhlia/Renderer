#pragma once
#include <vector>
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class Scene
{
public:
	uint32_t TeapotIndex;
	uint32_t TableIndex;
	uint32_t LightSourceIndex;

	std::vector<Model> SceneObjects;
	CameraHandler Camera;
	Light PointLight;

	void InitTestScene(ID3D11Device* InDevice);
	void RotateTestTeapot(float RollRotation);
};
