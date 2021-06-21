#pragma once
#include <stdint.h>
#include "d3d11.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "Texture.h"
using namespace DirectX;

struct Material
{
	Texture2D AlbedoTexrture;
};

class Model
{
	struct VertexDataType
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
	    XMFLOAT2 UV;
	};

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	std::vector<VertexDataType> VertexData;
	std::vector<uint32_t>		IndicesData;
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Scale;
public:
	Model() : VertexBuffer(nullptr), IndexBuffer(nullptr)
	{
		WorldMatrix = DirectX::XMMatrixIdentity();
		Position.x = 0;
		Position.y = 0;
		Position.z = 0;

		Scale.x = 1;
		Scale.y = 1;
		Scale.z = 1;
	};

	void LoadFromObj(std::string InFilePath, ID3D11Device* InDevice);
	void Initialize(ID3D11Device* InDevice);
	void Release();
	void ApplyBuffers(ID3D11DeviceContext* InDeviceContext);
	void UpdateWorldMatrix();

	uint32_t GetNumOfVertices() const 
	{
		return VertexData.size();
	}
	void SetPosition(float X, float Y, float Z);
	void SetScale(float X, float Y, float Z);

	XMMATRIX WorldMatrix;
	Material ModelMaterial;
};
