#include "Scene.h"

void Scene::InitTestScene(ID3D11Device* InDevice)
{
	PointLight.LightBufferData.LightPosition.x = 1.5;
	PointLight.LightBufferData.LightPosition.y = 2;
	PointLight.LightBufferData.LightPosition.z = 0;
	//PointLight.LightPosition.z = 15;

	std::wstring TableAmbientTexture(L"Resources/teapot_roughness.png");

	const XMFLOAT4 WhiteColor(1.f, 1.f, 1.f, 1.f);
	PointLight.LightBufferData.LightColor = WhiteColor;

	Model Teapot;
	Teapot.SetPosition(0, -0.02, 0);
	Teapot.SetScale(0.4, 0.02, 0.4);
	std::string TeapotObjFile("Resources/cube.obj");
	Teapot.ModelMaterial.AlbedoTexrture.Initialize(InDevice,L"Resources/steel.jpg");
	Teapot.LoadFromObj(TeapotObjFile, InDevice);
	SceneObjects.push_back(Teapot);
	TeapotIndex = SceneObjects.size() - 1;

	Model Table;
	std::string TableObjFile("Resources/teapot_s2.obj");
	Table.LoadFromObj(TableObjFile, InDevice);
	Table.ModelMaterial.AlbedoTexrture.Initialize(InDevice, TableAmbientTexture);
	SceneObjects.push_back(Table);
	TableIndex = SceneObjects.size() - 1;

	Model LightSource;
	std::string LightSourceObjFile("Resources/cube.obj");
	LightSource.LoadFromObj(LightSourceObjFile, InDevice);
	XMVECTOR LightSourceWorldTranslation = DirectX::XMLoadFloat3(&PointLight.LightBufferData.LightPosition);
	XMVECTOR LightSourceWorldScale = DirectX::XMLoadFloat3(&XMFLOAT3(0.1, 0.1, 0.1));
	XMVECTOR ZeroVector = DirectX::XMVectorZero();
	LightSource.WorldMatrix = DirectX::XMMatrixAffineTransformation(LightSourceWorldScale,ZeroVector, ZeroVector,LightSourceWorldTranslation);
	SceneObjects.push_back(LightSource);
	LightSourceIndex = SceneObjects.size() - 1;
}

void Scene::RotateTestTeapot(float RollRotation)
{
	/*Model & Teapot = SceneObjects[TeapotIndex];

	Teapot.WorldMatrix = XMMatrixRotationY(RollRotation);*/
}