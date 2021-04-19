#include "Scene.h"

void Scene::InitTestScene(ID3D11Device* InDevice)
{
	Model Teapot;
	std::string TeapotObjFile("Resources/teapot.obj");
	Teapot.LoadFromObj(TeapotObjFile, InDevice);

	SceneObjects.push_back(Teapot);
	TeapotIndex = SceneObjects.size() - 1;
}

void Scene::RotateTestTeapot(float RollRotation)
{
	Model & Teapot = SceneObjects[TeapotIndex];

	Teapot.WorldMatrix = XMMatrixRotationY(RollRotation);
}