#pragma once
#include "Shader.h"
#include <DirectXMath.h>
#include "Model.h"
#include "Resources.h"

constexpr ID3D11RenderTargetView* NULL_RT = nullptr;
constexpr ID3D11ShaderResourceView* NULL_SRV = nullptr;
constexpr ID3D11UnorderedAccessView* NULL_UAV = nullptr;

constexpr UINT NO_OFFSET = -1;

class SimpleColorShaderProgram
{
	struct MatrixBufferType
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};
	VertexShader SimpleColorVertexShader;
	PixelShader SimpleColorPixelShader;
	ID3D11InputLayout* SimpleColorVertexLayout;
	ID3D11Buffer* MatrixBuffer;
	ID3D11Buffer* ColorBuffer;
	DirectX::XMFLOAT4 ColorValue;
public:
	SimpleColorShaderProgram()
	{
		ColorValue = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	}

	void Init(ID3D11Device* InD3DDevice);

	void SetShaderParameters(ID3D11DeviceContext* InDeviceContext, DirectX::XMMATRIX InWorldMatrx, DirectX::XMMATRIX InViewMatrix,
		DirectX::XMMATRIX InProjectionMatrix, Model & InModelToDraw);

	void SetColor(const DirectX::XMFLOAT4 & InColorValue)
	{
		ColorValue = InColorValue;
	}

	void Draw(ID3D11DeviceContext* InDeviceContext, Model & InModelToDraw);
};

class BloomThresholdAndDownsample
{
	struct ThresholdShaderParam
	{
		float BrightnessThreshold;
		float Padding[3];
	};
	PixelShader ThresholdAndDownsamplePixelShader;
	VertexShader ThresholdAndDownsampleVertexShader;
	ID3D11InputLayout* QuadVertexLayout;
	ID3D11Buffer* VertexBuffer;
    ID3D11ShaderResourceView* BackBufferShaderResourceView;
	ID3D11Texture2D* RenderTargetTexture;
    ID3D11RenderTargetView* RenderTargetView;
    ID3D11ShaderResourceView* ShaderResourceView;
    ID3D11UnorderedAccessView* UnorderedAccessView;
	ID3D11SamplerState* SamplerState;
	DirectX::XMFLOAT2 Size;
public:
	BloomThresholdAndDownsample() {}

	ID3D11UnorderedAccessView* GetUAV()
	{
		return UnorderedAccessView;
	}

	ID3D11RenderTargetView* GetRTV()
	{
		return RenderTargetView;
	}

	ID3D11ShaderResourceView * GetSRV()
	{
		return ShaderResourceView;
	}

	void Init(ID3D11Device * InDevice, float BackBufferWidth, float BackBufferHeight, ID3D11ShaderResourceView* BackBuffer);
	void SetShaderParameters(ID3D11DeviceContext* InDeviceContext, float Threshold, ID3D11ShaderResourceView* InShaderResourceView);
	void Draw(ID3D11DeviceContext* InDeviceContext);
};


class QuadCompositeProgram
{
	struct QuadVertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 UV;
	};
	struct CompositeConstBufferParam
	{
		float Coefficient;
		float Padding[3];
	};
	VertexShader QuadCompositeVertexShader;
	RenderTarget RT;
	PixelShader QuadCompositePixelShader;
	ID3D11Buffer * ConstantBuffer;
	ID3D11InputLayout* QuadVertexLayout;
	ID3D11Buffer* VertexBuffer;
	ID3D11SamplerState* SamplerState;
public:
	QuadCompositeProgram() {};
	RenderTarget GetRenderTarget()
	{
		return RT;
	}
	void Init(ID3D11Device* InD3DDevice, float Width, float Height);

	void SetShaderParameters(ID3D11DeviceContext* InDeviceContext, ID3D11ShaderResourceView* InShaderResourceView1, ID3D11ShaderResourceView* InShaderResourceView2);

	void Draw(ID3D11DeviceContext* InDeviceContext);
};


class GaussianBlurShader
{
	struct GaussianBlurParams
	{
		int32_t Direction;
		float Padding[3];
	};

	PixelShader GaussianBlurPixelShader;
	VertexShader GaussianBlurVertexShader;
	RenderTarget RT;
	RenderTarget RTFinal;
	ID3D11Buffer * ConstantBuffers[2]; // According direction in GaussianBlurParams : 0 - horizontal, 1 - vertical
	ID3D11InputLayout* QuadVertexLayout;
	ID3D11Buffer* VertexBuffer;
	ID3D11SamplerState* SamplerState;
public:
	RenderTarget & GetRenderTarget()
	{
		return RT;
	};
	RenderTarget & GetRenderTargetFinal()
	{
		return RTFinal;
	};
	void Init(ID3D11Device* InD3DDevice, float Width, float Height);
	void SetShaderParams(ID3D11DeviceContext* InDeviceContext);
	void Draw(ID3D11DeviceContext* InDeviceContext, ID3D11ShaderResourceView * InTargetSRV, ID3D11RenderTargetView * InTargetRTV);
};

class TexturedQuadProgram
{
	PixelShader TexturedQuadPixelShader;
	VertexShader TexturedQuadVertexShader;
	RenderTarget RT;
	ID3D11InputLayout* QuadVertexLayout;
	ID3D11Buffer* VertexBuffer;
	ID3D11SamplerState* SamplerState;
public:
	RenderTarget & GetRenderTarget()
	{
		return RT;
	};
	void Init(ID3D11Device* InD3DDevice,float Width, float Height);
	void SetShaderParams(ID3D11DeviceContext* InDeviceContext, ID3D11Device* InD3DDevice,Texture2D & InTexture, float X, float Y, float Scale, ID3D11ShaderResourceView * InTargetSRV, float AR);
	void Draw(ID3D11DeviceContext* InDeviceContext, ID3D11RenderTargetView * InTargetRTV);

};

