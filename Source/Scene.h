#pragma once
#include <vector>
#include "Model.h"
#include "Camera.h"

class Scene
{
public:
	uint32_t TeapotIndex;
	std::vector<Model> SceneObjects;
	CameraHandler Camera;

	void InitTestScene(ID3D11Device* InDevice);
	void RotateTestTeapot(float RollRotation);
};
