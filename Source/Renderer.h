#pragma once

#include <windows.h>
#include "Shader.h"

#include <d3d11.h>      
#include <dxgi.h>       
#include <d3dcompiler.h> 
#include <DirectXMath.h>

#include "Scene.h"
#include "CoreMinimal.h"

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
	// Define the functionality of the rasterizer stage.
	ID3D11RasterizerState* D3DRasterizerState;
	D3D11_VIEWPORT D3DViewport;
};

class BasePassShaderProgram
{
	struct MatrixBufferType
	{
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Projection;
	};

	VertexShader BasePassVertexShader;
	PixelShader BasePassPixelShader;
	ID3D11InputLayout* BasePassVertexLayout;
	ID3D11Buffer* MatrixBuffer;

public:
	BasePassShaderProgram()
	{
	}

	void Init(ID3D11Device* InD3DDevice)
	{
		//Compile shader and initialize shader buffers 
		CHECK(InD3DDevice);
		BasePassPixelShader = PixelShader(L"Shaders/SimplePixel.hlsl", "ColorPixelShader");
		BasePassVertexShader = VertexShader(L"Shaders/SimpleVertex.hlsl", "ColorVertexShader");

		BasePassPixelShader.CompileShader();
		BasePassPixelShader.CreateShader(InD3DDevice);

		BasePassVertexShader.CompileShader();
		BasePassVertexShader.CreateShader(InD3DDevice);
		
		//Setup vertex layout
		HRESULT Result;
		D3D11_INPUT_ELEMENT_DESC VertexLayout[3];

		VertexLayout[0].SemanticName = "POSITION";
		VertexLayout[0].SemanticIndex = 0;
		VertexLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		VertexLayout[0].InputSlot = 0;
		VertexLayout[0].AlignedByteOffset = 0;
		VertexLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		VertexLayout[0].InstanceDataStepRate = 0;

		VertexLayout[1].SemanticName = "NORMAL";
		VertexLayout[1].SemanticIndex = 0;
		VertexLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		VertexLayout[1].InputSlot = 0;
		VertexLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		VertexLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		VertexLayout[1].InstanceDataStepRate = 0;

		VertexLayout[2].SemanticName = "TEXCOORD";
		VertexLayout[2].SemanticIndex = 0;
		VertexLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		VertexLayout[2].InputSlot = 0;
		VertexLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		VertexLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		VertexLayout[2].InstanceDataStepRate = 0;

		ID3DBlob* VertexShaderBinaries = BasePassVertexShader.GetBinaries();

		Result = InD3DDevice->CreateInputLayout(VertexLayout, sizeof(VertexLayout) / sizeof(VertexLayout[0]), VertexShaderBinaries->GetBufferPointer(), VertexShaderBinaries->GetBufferSize(),
			&BasePassVertexLayout);

		CHECK(SUCCEEDED(Result));

		D3D11_BUFFER_DESC MatrixBufferDesc = { };

		MatrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		MatrixBufferDesc.MiscFlags = 0;
		MatrixBufferDesc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		Result = InD3DDevice->CreateBuffer(&MatrixBufferDesc, NULL, &MatrixBuffer);
		CHECK(SUCCEEDED(Result));
	}

	void SetShaderParameters(ID3D11DeviceContext* InDeviceContext, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix,
		XMMATRIX InProjectionMatrix)
	{
		HRESULT Result;
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		unsigned int BufferNumber;
		MatrixBufferType MatrixBufferData;



		// Transpose the matrices to prepare them for the shader.
		XMMATRIX TransposedWorld = DirectX::XMMatrixTranspose(InWorldMatrix);
		XMMATRIX TransposedView = DirectX::XMMatrixTranspose(InViewMatrix);
		XMMATRIX TransposedProjection = DirectX::XMMatrixTranspose(InProjectionMatrix);

		MatrixBufferData.World = TransposedWorld;
		MatrixBufferData.View = TransposedView;
		MatrixBufferData.Projection = TransposedProjection;

		// Lock the constant buffer so it can be written to.
		//Result = InDeviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		InDeviceContext->UpdateSubresource(MatrixBuffer, 0, 0, &MatrixBufferData, 0, 0);

		//CHECK(SUCCEEDED(Result));

		// Get a pointer to the data in the constant buffer.
		//MatrixBufferData = (MatrixBufferType*)MappedResource.pData;

		// Copy the matrices into the constant buffer.
		
		// Unlock the constant buffer.
		//InDeviceContext->Unmap(MatrixBuffer, 0);

		// Set the position of the constant buffer in the vertex shader.
		BufferNumber = 0;

		// Now set the constant buffer in the vertex shader with the updated values.
		InDeviceContext->VSSetConstantBuffers(BufferNumber, 1, &MatrixBuffer);
	}

	void Draw(ID3D11DeviceContext* InDeviceContext, Model & InModelToDraw)
	{
		InDeviceContext->IASetInputLayout(BasePassVertexLayout);

		// Set the vertex and pixel shaders that will be used to render this triangle.
		InDeviceContext->VSSetShader(BasePassVertexShader.GetD3DVertexShader(), NULL, 0);
		InDeviceContext->PSSetShader(BasePassPixelShader.GetD3DPixelShader(), NULL, 0);

		// Render the triangle.
		InDeviceContext->Draw(InModelToDraw.GetNumOfVertices(), 0);
	}
};

class DXRenderer
{
public:
	DirectXSpec Spec;
	BasePassShaderProgram BasePassProgram;
	Scene MainScene;
	XMMATRIX ProjectionMatrix;

	void InitRendererState(HWND & TargetWindow);

	void BeginRendering();
	void EndRendering();

	void RenderFrame(float InDelta);
	void RenderTeapot();

	DXRenderer(const DirectXSpec & InSpec) : Spec(InSpec)
	{

	};
	DXRenderer() {};
	~DXRenderer() {};
};
