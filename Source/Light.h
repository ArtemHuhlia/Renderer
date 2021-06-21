#pragma once
#include "d3d11.h"
#include <DirectXMath.h>

struct LightShaderInfo
{
	//DirectX::XMFLOAT3 LightDirection;
	DirectX::XMFLOAT4 LightColor;
	DirectX::XMFLOAT3 LightPosition;
	DirectX::XMFLOAT3 ViewPosition;
	DirectX::XMFLOAT2 Padding;
};

struct Light
{
	LightShaderInfo LightBufferData;
};
