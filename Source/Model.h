#pragma once
#include <stdint.h>
#include "d3d11.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
using namespace DirectX;

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
public:
	Model() : VertexBuffer(nullptr), IndexBuffer(nullptr)
	{
		WorldMatrix = XMMatrixIdentity();
	};

	void LoadFromObj(std::string InFilePath, ID3D11Device* InDevice);
	void Initialize(ID3D11Device* InDevice);
	void Release();
	void ApplyBuffers(ID3D11DeviceContext* InDeviceContext);

	uint32_t GetNumOfVertices() const 
	{
		return VertexData.size();
	}

	XMMATRIX WorldMatrix;
};
