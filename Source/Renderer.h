#pragma once

#include <windows.h>
#include "Shader.h"

#include <d3d11.h>      
#include <dxgi.h>       
#include <d3dcompiler.h> 
#include <DirectXMath.h>

#include "Scene.h"
#include "Light.h"
#include "CoreMinimal.h"
#include "BasePassShader.h"
#include "MiscShaders.h"

using namespace DirectX;

struct DirectXSpec
{
	// Direct3D device and swap chain.
	ID3D11Device* D3DDevice;
	ID3D11DeviceContext* D3DDeviceContext;
	IDXGISwapChain* D3DSwapChain;

	// Render target view for the back buffer of the swap chain.
	ID3D11RenderTargetView* D3DRenderTargetView;

	// Depth/stencil view for use as a depth buffer.
	ID3D11DepthStencilView* D3DDepthStencilView;
	// A texture to associate to the depth stencil view.
	ID3D11Texture2D* D3DDepthStencilBuffer;

	// Define the functionality of the depth/stencil stages.
	ID3D11DepthStencilState* D3DDepthStencilState;
	ID3D11DepthStencilState* D3DNoDepthStencilState;
	// Define the functionality of the rasterizer stage.
	ID3D11RasterizerState* D3DRasterizerState;
	D3D11_VIEWPORT D3DViewport;
 
	//Define Blend States
	ID3D11BlendState* AlphaEnableBlendingState;
	ID3D11BlendState* AlphaDisableBlendingState;

	void TurnOnAlphaBlending()
	{
		float blendFactor[4];


		// Setup the blend factor.
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;

		// Turn on the alpha blending.
		D3DDeviceContext->OMSetBlendState(AlphaEnableBlendingState, blendFactor, 0xffffffff);

		return;
	}


	void TurnOffAlphaBlending()
	{
		float blendFactor[4];


		// Setup the blend factor.
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;

		// Turn off the alpha blending.
		D3DDeviceContext->OMSetBlendState(AlphaDisableBlendingState, blendFactor, 0xffffffff);

		return;
	}

};

class DXRenderer
{
public:
	DirectXSpec Spec;
	BasePassShaderProgram BasePassProgram;
	SimpleColorShaderProgram LightSourceShaderProgram;
	BloomThresholdAndDownsample BloomTresholdAndDownsampleProgram;
	QuadCompositeProgram BloomQuadCompositeProgram;
	GaussianBlurShader GaussianBlurShaderProgram;
	TexturedQuadProgram LensFlaresShaderPrograms[2];
	Scene MainScene;
	XMMATRIX ProjectionMatrix;

	void InitRendererState(HWND & TargetWindow);

	void BeginRendering();
	void EndRendering();

	void RenderFrame(float InDelta);
	void RenderTeapot();
	void RenderTable();
	void RenderBloom();
	void RenderLensFlares();
	void RenderLightSource();

	DXRenderer(const DirectXSpec & InSpec) : Spec(InSpec)
	{

	};
	DXRenderer() {};
	~DXRenderer() {};
};
