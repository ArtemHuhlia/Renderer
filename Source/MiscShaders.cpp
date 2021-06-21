#include "MiscShaders.h"
#include "CoreMinimal.h"

struct QuadVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};

std::vector<QuadVertex> ScreenQuadVertexData = {
    QuadVertex{ DirectX::XMFLOAT3(-1.f,  1.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f) },//-1.f,  1.f, 0.f,   0.f, 0.f 
    QuadVertex{ DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f) },// 1.f, -1.f, 0.f, 1.f, 1.f 
    QuadVertex{ DirectX::XMFLOAT3(-1.f, -1.f, 0.f), DirectX::XMFLOAT2(0.f, 1.f) },//-1.f, -1.f, 0.f, 0.f, 1.f
    QuadVertex{ DirectX::XMFLOAT3(-1.f,  1.f, 0.f), DirectX::XMFLOAT2(0.f, 0.f) },//-1.f,  1.f, 0.f, 0.f, 0.f 
    QuadVertex{ DirectX::XMFLOAT3(1.f,  1.f, 0.f), DirectX::XMFLOAT2(1.f, 0.f) },//1.f,  1.f, 0.f, 1.f, 0.f
    QuadVertex{ DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f) }//1.f, -1.f, 0.f, 1.f, 1.f
	};


void SimpleColorShaderProgram::Init(ID3D11Device* InD3DDevice)
{
	//Compile shader and initialize shader buffers 
	CHECK(InD3DDevice);
	SimpleColorPixelShader = PixelShader(L"Shaders/SimplePixel.hlsl", "ColorPixelShader");
	SimpleColorVertexShader = VertexShader(L"Shaders/SimpleVertex.hlsl", "ColorVertexShader");

	SimpleColorPixelShader.CompileShader();
	SimpleColorPixelShader.CreateShader(InD3DDevice);

	SimpleColorVertexShader.CompileShader();
	SimpleColorVertexShader.CreateShader(InD3DDevice);

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

	ID3DBlob* VertexShaderBinaries = SimpleColorVertexShader.GetBinaries();

	Result = InD3DDevice->CreateInputLayout(VertexLayout, sizeof(VertexLayout) / sizeof(VertexLayout[0]), VertexShaderBinaries->GetBufferPointer(), VertexShaderBinaries->GetBufferSize(),
		&SimpleColorVertexLayout);

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

	D3D11_BUFFER_DESC ColorBufferDesc = { };

	ColorBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ColorBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
	ColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//ColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ColorBufferDesc.MiscFlags = 0;
	ColorBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	Result = InD3DDevice->CreateBuffer(&ColorBufferDesc, NULL, &ColorBuffer);
	CHECK(SUCCEEDED(Result));
}

void SimpleColorShaderProgram::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, DirectX::XMMATRIX InWorldMatrix, DirectX::XMMATRIX InViewMatrix,
	DirectX::XMMATRIX InProjectionMatrix, Model & InModelToDraw)
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

	// Set the position of the constant buffer in the vertex shader.
	VSBufferNumber = 0;
	PSBufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	InDeviceContext->VSSetConstantBuffers(VSBufferNumber, 1, &MatrixBuffer);

	// Color buffer setuping
	InDeviceContext->UpdateSubresource(ColorBuffer, 0, 0, &ColorValue, 0, 0);

	InDeviceContext->PSSetConstantBuffers(PSBufferNumber, 1, &ColorBuffer);
}

void SimpleColorShaderProgram::Draw(ID3D11DeviceContext* InDeviceContext, Model & InModelToDraw)
{
	InDeviceContext->IASetInputLayout(SimpleColorVertexLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	InDeviceContext->VSSetShader(SimpleColorVertexShader.GetD3DVertexShader(), NULL, 0);
	InDeviceContext->PSSetShader(SimpleColorPixelShader.GetD3DPixelShader(), NULL, 0);

	// Render the triangle.
	InDeviceContext->Draw(InModelToDraw.GetNumOfVertices(), 0);
}

void BloomThresholdAndDownsample::Init(ID3D11Device * InDevice, float BackBufferWidth, float BackBufferHeight, ID3D11ShaderResourceView* BackBuffer)
{
	ThresholdAndDownsamplePixelShader = PixelShader(L"Shaders/BloomThresholdAndDownsample.hlsl", "PSMain");
	ThresholdAndDownsamplePixelShader.CompileShader();
	ThresholdAndDownsamplePixelShader.CreateShader(InDevice);

	ThresholdAndDownsampleVertexShader = VertexShader(L"Shaders/BloomThresholdAndDownsample.hlsl", "VSMain");
	ThresholdAndDownsampleVertexShader.CompileShader();
	ThresholdAndDownsampleVertexShader.CreateShader(InDevice);

	CHECK(BackBuffer);
	BackBufferShaderResourceView = BackBuffer;
	
	Size.x = BackBufferWidth;
	Size.y = BackBufferHeight;

	D3D11_TEXTURE2D_DESC TextureDesc = {};
	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};

	// 1. Create Texture2D
	TextureDesc.Width = Size.x;
	TextureDesc.Height = Size.y;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;
	
	HRESULT Result = InDevice->CreateTexture2D(&TextureDesc, NULL, &RenderTargetTexture);
	CHECK(SUCCEEDED(Result));

	// 2. Create RTV
	RenderTargetViewDesc.Format = TextureDesc.Format;
	RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetViewDesc.Texture2D.MipSlice = 0;

	Result = InDevice->CreateRenderTargetView(RenderTargetTexture, &RenderTargetViewDesc, &RenderTargetView);
	CHECK(SUCCEEDED(Result));

	// 3. Create SRV
	SRVDesc.Format = TextureDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	Result = InDevice->CreateShaderResourceView(RenderTargetTexture, &SRVDesc, &ShaderResourceView);
	CHECK(SUCCEEDED(Result));

	// 4. Create UAV
	UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	UAVDesc.Texture2D.MipSlice = 0;

	Result = InDevice->CreateUnorderedAccessView(RenderTargetTexture, &UAVDesc, &UnorderedAccessView);
	CHECK(SUCCEEDED(Result));

	D3D11_BUFFER_DESC VertexBufferDesc;

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(QuadVertex) * ScreenQuadVertexData.size();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexSubresData;

	VertexSubresData.pSysMem = ScreenQuadVertexData.data();
	VertexSubresData.SysMemPitch = 0;
	VertexSubresData.SysMemSlicePitch = 0;

	Result = InDevice->CreateBuffer(&VertexBufferDesc, &VertexSubresData, &VertexBuffer);
	CHECK(SUCCEEDED(Result));

	D3D11_INPUT_ELEMENT_DESC VertexLayouts[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3DBlob* VertexShaderBinaries = ThresholdAndDownsampleVertexShader.GetBinaries();

	Result = InDevice->CreateInputLayout(VertexLayouts, 2, VertexShaderBinaries->GetBufferPointer(), VertexShaderBinaries->GetBufferSize(), &QuadVertexLayout);
	CHECK(SUCCEEDED(Result));

	D3D11_SAMPLER_DESC SampDesc = { };
	SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SampDesc.MinLOD = 0;
	SampDesc.MaxLOD = 1;

	Result = InDevice->CreateSamplerState(&SampDesc, &SamplerState);
	CHECK(SUCCEEDED(Result));
}

void BloomThresholdAndDownsample::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, float Threshold, ID3D11ShaderResourceView* InShaderResourceView)
{
	InDeviceContext->PSSetShader(ThresholdAndDownsamplePixelShader.GetD3DPixelShader(), 0, 0);
	InDeviceContext->VSSetShader(ThresholdAndDownsampleVertexShader.GetD3DVertexShader(), 0, 0);

	const uint32_t VertexBufferStride = sizeof(QuadVertex);
	const uint32_t VertexBufferOffset = 0u;

	InDeviceContext->IASetVertexBuffers(0u, 1u, &VertexBuffer, &VertexBufferStride, &VertexBufferOffset);
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	InDeviceContext->IASetInputLayout(QuadVertexLayout);

	//Set threshold
	bool IsThresholdValid = Threshold >= 0.f && Threshold <= 1.f;
	CHECK(IsThresholdValid);
	ThresholdShaderParam ConstantBufferParams;
	ConstantBufferParams.BrightnessThreshold = Threshold;

	//Set SRV and UAV
	CHECK(BackBufferShaderResourceView);
	InDeviceContext->PSSetShaderResources(0, 1, &InShaderResourceView);
}

void BloomThresholdAndDownsample::Draw(ID3D11DeviceContext* InDeviceContext)
{
	InDeviceContext->Draw(ScreenQuadVertexData.size(), 0); // divide by 16 since we have 8 thread splitting in shader: [numthreads(8, 8, 1)] 
}

void QuadCompositeProgram::Init(ID3D11Device* InD3DDevice, float Width, float Height)
{
	RT.Init(InD3DDevice, Width, Height);
	QuadCompositePixelShader = PixelShader(L"Shaders/QuadCompisite.hlsl", "PSMain");
	QuadCompositeVertexShader = VertexShader(L"Shaders/QuadCompisite.hlsl", "VSMain");

	QuadCompositePixelShader.CompileShader();
	QuadCompositePixelShader.CreateShader(InD3DDevice);

	QuadCompositeVertexShader.CompileShader();
	QuadCompositeVertexShader.CreateShader(InD3DDevice);

	D3D11_BUFFER_DESC VertexBufferDesc;

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(QuadVertex) * ScreenQuadVertexData.size();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexSubresData;

	VertexSubresData.pSysMem = ScreenQuadVertexData.data();
	VertexSubresData.SysMemPitch = 0;
	VertexSubresData.SysMemSlicePitch = 0;

	HRESULT Result = InD3DDevice->CreateBuffer(&VertexBufferDesc, &VertexSubresData, &VertexBuffer);
	CHECK(SUCCEEDED(Result));

	D3D11_INPUT_ELEMENT_DESC VertexLayouts[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3DBlob* VertexShaderBinaries = QuadCompositeVertexShader.GetBinaries();

	Result = InD3DDevice->CreateInputLayout(VertexLayouts, 2, VertexShaderBinaries->GetBufferPointer(), VertexShaderBinaries->GetBufferSize(), &QuadVertexLayout);
	CHECK(SUCCEEDED(Result));


	D3D11_BUFFER_DESC ConstantBufferDesc = {};

	ConstantBufferDesc.ByteWidth = sizeof(CompositeConstBufferParam);
	ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Result = InD3DDevice->CreateBuffer(&ConstantBufferDesc, NULL, &ConstantBuffer);
	CHECK(SUCCEEDED(Result));

	D3D11_SAMPLER_DESC SampDesc = { };
	SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SampDesc.MinLOD = 0;
	SampDesc.MaxLOD = 1;

	Result = InD3DDevice->CreateSamplerState(&SampDesc, &SamplerState);
	CHECK(SUCCEEDED(Result));

}

void QuadCompositeProgram::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, ID3D11ShaderResourceView* InShaderResourceView1, ID3D11ShaderResourceView* InShaderResourceView2)
{
	InDeviceContext->VSSetShader(QuadCompositeVertexShader.GetD3DVertexShader(), 0, 0);
    InDeviceContext->PSSetShader(QuadCompositePixelShader.GetD3DPixelShader(), 0, 0);

	const uint32_t VertexBufferStride = sizeof(QuadVertex);
	const uint32_t VertexBufferOffset = 0u;

	InDeviceContext->IASetVertexBuffers(0u, 1u, &VertexBuffer, &VertexBufferStride, &VertexBufferOffset);
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	InDeviceContext->IASetInputLayout(QuadVertexLayout);

	ID3D11ShaderResourceView* ShaderResourceViews[] =
	{
		InShaderResourceView1,
		InShaderResourceView2
	};
	InDeviceContext->PSSetShaderResources(0, 2, ShaderResourceViews);
	InDeviceContext->PSSetSamplers(0, 1, &SamplerState);

	CompositeConstBufferParam ConstBuffer = { 0.75 };
	InDeviceContext->UpdateSubresource(ConstantBuffer, 0, 0, &ConstBuffer, 0, 0);
	InDeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
}

void QuadCompositeProgram::Draw(ID3D11DeviceContext* InDeviceContext)
{
	InDeviceContext->Draw(ScreenQuadVertexData.size(), 0);
}

void GaussianBlurShader::Init(ID3D11Device* InD3DDevice, float Width, float Height)
{
	RT.Init(InD3DDevice, Width, Height);
	RTFinal.Init(InD3DDevice, Width, Height);

	GaussianBlurPixelShader = PixelShader(L"Shaders/Blur.hlsl", "PSMain");
	GaussianBlurPixelShader.CompileShader();
	GaussianBlurPixelShader.CreateShader(InD3DDevice);

	GaussianBlurVertexShader = VertexShader(L"Shaders/Blur.hlsl", "VSMain");
	GaussianBlurVertexShader.CompileShader();
	GaussianBlurVertexShader.CreateShader(InD3DDevice);

	D3D11_BUFFER_DESC VertexBufferDesc;

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(QuadVertex) * ScreenQuadVertexData.size();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexSubresData;

	VertexSubresData.pSysMem = ScreenQuadVertexData.data();
	VertexSubresData.SysMemPitch = 0;
	VertexSubresData.SysMemSlicePitch = 0;

	HRESULT Result = InD3DDevice->CreateBuffer(&VertexBufferDesc, &VertexSubresData, &VertexBuffer);
	CHECK(SUCCEEDED(Result));

	D3D11_INPUT_ELEMENT_DESC VertexLayouts[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3DBlob* VertexShaderBinaries = GaussianBlurVertexShader.GetBinaries();

	Result = InD3DDevice->CreateInputLayout(VertexLayouts, 2, VertexShaderBinaries->GetBufferPointer(), VertexShaderBinaries->GetBufferSize(), &QuadVertexLayout);
	CHECK(SUCCEEDED(Result));


	D3D11_BUFFER_DESC BufferDesc = {};

	BufferDesc.ByteWidth = sizeof(GaussianBlurParams);
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create buffers for horizontal and vertical blur
	Result = InD3DDevice->CreateBuffer(&BufferDesc, NULL, &ConstantBuffers[0]);
	CHECK(SUCCEEDED(Result));

	Result = InD3DDevice->CreateBuffer(&BufferDesc, NULL, &ConstantBuffers[1]);
	CHECK(SUCCEEDED(Result));
}

void GaussianBlurShader::SetShaderParams(ID3D11DeviceContext* InDeviceContext)
{
	InDeviceContext->PSSetShader(GaussianBlurPixelShader.GetD3DPixelShader(), 0, 0);
	InDeviceContext->VSSetShader(GaussianBlurVertexShader.GetD3DVertexShader(), 0, 0);
	// Set constant buffers
	// Compute Gaussian kernel
	const uint32_t VertexBufferStride = sizeof(QuadVertex);
	const uint32_t VertexBufferOffset = 0u;

	InDeviceContext->IASetVertexBuffers(0u, 1u, &VertexBuffer, &VertexBufferStride, &VertexBufferOffset);
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	InDeviceContext->IASetInputLayout(QuadVertexLayout);

	GaussianBlurParams ConstantBufferHor;
	ConstantBufferHor.Direction = 1;
	GaussianBlurParams ConstantBufferVert;
	ConstantBufferVert.Direction = 0;

	InDeviceContext->UpdateSubresource(ConstantBuffers[0], 0, 0, &ConstantBufferHor, 0, 0);
	InDeviceContext->UpdateSubresource(ConstantBuffers[1], 0, 0, &ConstantBufferVert, 0, 0);
}

void GaussianBlurShader::Draw(ID3D11DeviceContext* InDeviceContext, ID3D11ShaderResourceView * InTargetSRV, ID3D11RenderTargetView * InTargetRTV)
{
	ID3D11ShaderResourceView * SRVs[2] = {InTargetSRV, RT.ShaderResourceView};
	ID3D11RenderTargetView * RTVs[2] = {RT.RenderTargetView, InTargetRTV};

	for (size_t i = 0; i < 2; ++i)
	{
		InDeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffers[i]);
		InDeviceContext->PSSetShaderResources(0, 1, &SRVs[i]);
		InDeviceContext->OMSetRenderTargets(1, &RTVs[i], nullptr);

		InDeviceContext->Draw(ScreenQuadVertexData.size(), 0);

		// Unbind RTV and SRV
        InDeviceContext->PSSetShaderResources(0, 1, &NULL_SRV);
		InDeviceContext->OMSetRenderTargets(1, &NULL_RT, nullptr);
	}
} 

void TexturedQuadProgram::Init(ID3D11Device* InD3DDevice,float Width, float Height)
{
	RT.Init(InD3DDevice, Width, Height);

	TexturedQuadPixelShader = PixelShader(L"Shaders/TexturedQuad.hlsl", "PSMain");
	TexturedQuadPixelShader.CompileShader();
	TexturedQuadPixelShader.CreateShader(InD3DDevice);

	TexturedQuadVertexShader = VertexShader(L"Shaders/TexturedQuad.hlsl", "VSMain");
	TexturedQuadVertexShader.CompileShader();
	TexturedQuadVertexShader.CreateShader(InD3DDevice);

	D3D11_SAMPLER_DESC SampDesc = { };
	SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SampDesc.MinLOD = 0;
	SampDesc.MaxLOD = 1;

	HRESULT Result = InD3DDevice->CreateSamplerState(&SampDesc, &SamplerState);
	CHECK(SUCCEEDED(Result));
}

void TexturedQuadProgram::SetShaderParams(ID3D11DeviceContext* InDeviceContext, ID3D11Device* InD3DDevice,Texture2D & InTexture, float X, float Y, float Scale, ID3D11ShaderResourceView * InTargetSRV, float AR)
{
	std::vector<QuadVertex> TexturedQuadVertexData;
	QuadVertex V0;
	V0.Position = DirectX::XMFLOAT3((X - Scale) / AR, Y + Scale, 0);
	V0.UV = DirectX::XMFLOAT2(0, 0);
	QuadVertex V1;
	V1.Position = DirectX::XMFLOAT3((X + Scale) / AR, Y - Scale, 0);
	V1.UV = DirectX::XMFLOAT2(1, 1);
	QuadVertex V2;
	V2.Position = DirectX::XMFLOAT3((X - Scale) / AR, Y - Scale, 0);
	V2.UV = DirectX::XMFLOAT2(0, 1);
	QuadVertex V3;
	V3.Position = DirectX::XMFLOAT3((X + Scale) / AR, Y + Scale, 0);
	V3.UV = DirectX::XMFLOAT2(1, 0);

	TexturedQuadVertexData.push_back(V0);
	TexturedQuadVertexData.push_back(V1);
	TexturedQuadVertexData.push_back(V2);
	TexturedQuadVertexData.push_back(V0);
	TexturedQuadVertexData.push_back(V3);
	TexturedQuadVertexData.push_back(V1);

	D3D11_BUFFER_DESC VertexBufferDesc;

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(QuadVertex) * TexturedQuadVertexData.size();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexSubresData;

	VertexSubresData.pSysMem = TexturedQuadVertexData.data();
	VertexSubresData.SysMemPitch = 0;
	VertexSubresData.SysMemSlicePitch = 0;

	HRESULT Result = InD3DDevice->CreateBuffer(&VertexBufferDesc, &VertexSubresData, &VertexBuffer);
	CHECK(SUCCEEDED(Result));

	D3D11_INPUT_ELEMENT_DESC VertexLayouts[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3DBlob* VertexShaderBinaries = TexturedQuadVertexShader.GetBinaries();

	Result = InD3DDevice->CreateInputLayout(VertexLayouts, 2, VertexShaderBinaries->GetBufferPointer(), VertexShaderBinaries->GetBufferSize(), &QuadVertexLayout);
	CHECK(SUCCEEDED(Result));

	InDeviceContext->VSSetShader(TexturedQuadVertexShader.GetD3DVertexShader(), 0, 0);
    InDeviceContext->PSSetShader(TexturedQuadPixelShader.GetD3DPixelShader(), 0, 0);

	const uint32_t VertexBufferStride = sizeof(QuadVertex);
	const uint32_t VertexBufferOffset = 0u;

	InDeviceContext->IASetVertexBuffers(0u, 1u, &VertexBuffer, &VertexBufferStride, &VertexBufferOffset);
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	InDeviceContext->IASetInputLayout(QuadVertexLayout);

	InDeviceContext->PSSetShaderResources(0, 1, &InTargetSRV);
	ID3D11ShaderResourceView * TextureSRV = InTexture.GetTexture();
	InDeviceContext->PSSetShaderResources(1, 1, &TextureSRV);
}

void TexturedQuadProgram::Draw(ID3D11DeviceContext* InDeviceContext, ID3D11RenderTargetView * InTargetRTV)
{
	InDeviceContext->OMSetRenderTargets(1, &InTargetRTV, nullptr);
	InDeviceContext->Draw(6, 0);
	InDeviceContext->OMSetRenderTargets(1, &NULL_RT, nullptr);
	ID3D11ShaderResourceView* ShaderResourceViews[] =
	{
		NULL_SRV,
		NULL_SRV
	};

    InDeviceContext->PSSetShaderResources(0, 2, ShaderResourceViews);
}