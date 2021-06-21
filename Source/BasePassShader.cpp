#include "BasePassShader.h"
#include "CoreMinimal.h"

void BasePassShaderProgram::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, DirectX::XMMATRIX InWorldMatrix, DirectX::XMMATRIX InViewMatrix,
	DirectX::XMMATRIX InProjectionMatrix, Model & InModelToDraw, Light & InLight)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	unsigned int VSBufferNumber;
	unsigned int PSBufferNumber;
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
	VSBufferNumber = 0;
	PSBufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	InDeviceContext->VSSetConstantBuffers(VSBufferNumber, 1, &MatrixBuffer);

	// Light buffer setuping
	InDeviceContext->UpdateSubresource(LightBuffer, 0, 0, &InLight.LightBufferData, 0, 0);

	InDeviceContext->PSSetConstantBuffers(PSBufferNumber, 1, &LightBuffer);
	ID3D11ShaderResourceView * SRV = InModelToDraw.ModelMaterial.AlbedoTexrture.GetTexture();
	InDeviceContext->PSSetShaderResources(0, 1, &SRV);

}

void BasePassShaderProgram::Init(ID3D11Device* InD3DDevice, float Width, float Height)
{
	//Compile shader and initialize shader buffers 
	CHECK(InD3DDevice);
	BasePassPixelShader = PixelShader(L"Shaders/BasePassPixel.hlsl", "ColorPixelShader");
	BasePassVertexShader = VertexShader(L"Shaders/BasePassVertex.hlsl", "ColorVertexShader");

	BasePassPixelShader.CompileShader();
	BasePassPixelShader.CreateShader(InD3DDevice);

	BasePassVertexShader.CompileShader();
	BasePassVertexShader.CreateShader(InD3DDevice);

	BasePassRT.Init(InD3DDevice, Width, Height);

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

	D3D11_BUFFER_DESC LightBufferDesc = { };

	LightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	LightBufferDesc.ByteWidth = sizeof(LightShaderInfo);
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBufferDesc.MiscFlags = 0;
	LightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	Result = InD3DDevice->CreateBuffer(&LightBufferDesc, NULL, &LightBuffer);
	CHECK(SUCCEEDED(Result));
}
 
void BasePassShaderProgram::Draw(ID3D11DeviceContext* InDeviceContext, Model & InModelToDraw)
{
	InDeviceContext->IASetInputLayout(BasePassVertexLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	InDeviceContext->VSSetShader(BasePassVertexShader.GetD3DVertexShader(), NULL, 0);
	InDeviceContext->PSSetShader(BasePassPixelShader.GetD3DPixelShader(), NULL, 0);

	// Render the triangle.
	InDeviceContext->Draw(InModelToDraw.GetNumOfVertices(), 0);
}

