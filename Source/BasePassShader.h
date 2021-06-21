#pragma once
#include <d3d11.h>      
#include <DirectXMath.h>
#include "Model.h"
#include "Light.h"
#include "Shader.h"
#include "Resources.h"

class BasePassShaderProgram
{
	struct MatrixBufferType
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};

	VertexShader BasePassVertexShader;
	PixelShader BasePassPixelShader;
	ID3D11InputLayout* BasePassVertexLayout;
	ID3D11Buffer* MatrixBuffer;
	ID3D11Buffer * LightBuffer;
	RenderTarget BasePassRT;
public:
	BasePassShaderProgram()
	{

	}

	void Init(ID3D11Device* InD3DDevice, float Width, float Height);

	void SetShaderParameters(ID3D11DeviceContext* InDeviceContext, DirectX::XMMATRIX InWorldMatrix, DirectX::XMMATRIX InViewMatrix,
		DirectX::XMMATRIX InProjectionMatrix, Model & InModelToDraw, Light & InLight);

	RenderTarget & GetRenderTargetToDraw()
	{
		return BasePassRT;
	}

	void Draw(ID3D11DeviceContext* InDeviceContext, Model & InModelToDraw);
};
