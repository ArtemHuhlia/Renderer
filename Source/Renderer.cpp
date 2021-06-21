#include "Renderer.h"

#include "CoreMinimal.h"
#include "CameraController.h"
#define ENABLE_VSYNC 0

#define SCREEN_DEPTH 1000.0f
#define SCREEN_NEAR  0.1f


void DXRenderer::InitRendererState(HWND & TargetWindow)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	RECT TargetWindowRect;
	GetClientRect(TargetWindow, &TargetWindowRect);

	const XMUINT2 SwapChainBufferSize = {
		static_cast<uint32_t>(TargetWindowRect.right - TargetWindowRect.left),
		static_cast<uint32_t>(TargetWindowRect.bottom - TargetWindowRect.top),
	};
	CHECK(SwapChainBufferSize.x > 0 && SwapChainBufferSize.y > 0);
	constexpr DXGI_RATIONAL RefreshRate = { 0, 0 };

	SwapChainDesc.BufferCount = 1;
    SwapChainDesc.BufferDesc.Width = SwapChainBufferSize.x;
    SwapChainDesc.BufferDesc.Height = SwapChainBufferSize.y;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate = RefreshRate;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = TargetWindow;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    SwapChainDesc.Windowed = false;

	D3D_FEATURE_LEVEL OutFeatureLvl;
	UINT Flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(RE_DEBUG)
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT Result; // will be used one by one for validating each step
	Result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		Flags,
		0u,
		0u,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&Spec.D3DSwapChain,
		&Spec.D3DDevice,
		&OutFeatureLvl, //TODO: mb we will need this in the future?
		&Spec.D3DDeviceContext
	);



	// HACK: Its not ok. try to adjust feature level if somethis goes wrong
	CHECK(Result == S_OK && Spec.D3DSwapChain && Spec.D3DDevice && Spec.D3DDeviceContext); 

	// Setup Swap Chain
	ID3D11Texture2D * BackBufferTexture;
	Result = Spec.D3DSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&BackBufferTexture
	);
	CHECK(Result == S_OK);

	Result = Spec.D3DDevice->CreateRenderTargetView(BackBufferTexture, nullptr, &Spec.D3DRenderTargetView);
	CHECK(Result == S_OK);

	//BackBufferTexture->Release();

	// Setup depth/stencil state.
	// Set up the description of the depth buffer.
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));

	DepthBufferDesc.Width = SwapChainBufferSize.x;
	DepthBufferDesc.Height = SwapChainBufferSize.y;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count = 1;
	DepthBufferDesc.SampleDesc.Quality = 0;
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	Result = Spec.D3DDevice->CreateTexture2D(&DepthBufferDesc, NULL, &Spec.D3DDepthStencilBuffer);
	CHECK(SUCCEEDED(Result));

	D3D11_DEPTH_STENCIL_DESC DepthStencilStateDesc;
	ZeroMemory(&DepthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	DepthStencilStateDesc.DepthEnable = TRUE;
	DepthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//TODO: do we rly need stencil. set false now
	DepthStencilStateDesc.StencilEnable = FALSE; //TRUE;
	//depthStencilDesc.StencilReadMask = 0xFF;
	//depthStencilDesc.StencilWriteMask = 0xFF;

	//// Stencil operations if pixel is front-facing.
	//depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing.
	//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	Result = Spec.D3DDevice->CreateDepthStencilState(&DepthStencilStateDesc, &Spec.D3DDepthStencilState);
	CHECK(Result == S_OK);

	DepthStencilStateDesc.DepthEnable = FALSE;
	Result = Spec.D3DDevice->CreateDepthStencilState(&DepthStencilStateDesc, &Spec.D3DNoDepthStencilState);
	CHECK(Result == S_OK);


	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	// Initialize the depth stencil view.
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));

	// Set up the depth stencil view description.
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	Result = Spec.D3DDevice->CreateDepthStencilView(Spec.D3DDepthStencilBuffer, &DepthStencilViewDesc, &Spec.D3DDepthStencilView);
	CHECK(Result == S_OK);

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	//Spec.D3DDeviceContext->OMSetRenderTargets(1, &Spec.D3DRenderTargetView, Spec.D3DDepthStencilView);

	// Setup rasterizer state.
	D3D11_RASTERIZER_DESC RasterizerDesc;
	ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	RasterizerDesc.AntialiasedLineEnable = false;
	RasterizerDesc.CullMode = D3D11_CULL_NONE;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.0f;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.MultisampleEnable = false;
	RasterizerDesc.ScissorEnable = false;
	RasterizerDesc.SlopeScaledDepthBias = 0.0f;

	Result = Spec.D3DDevice->CreateRasterizerState(&RasterizerDesc, &Spec.D3DRasterizerState);

	CHECK(Result == S_OK);

	// Now set the rasterizer state.
	Spec.D3DDeviceContext->RSSetState(Spec.D3DRasterizerState);

	// Setup viewport 
    Spec.D3DViewport.Width =	static_cast<float>(SwapChainBufferSize.x);
    Spec.D3DViewport.Height =	static_cast<float>(SwapChainBufferSize.y);
    Spec.D3DViewport.TopLeftX = 0.0f;
	Spec.D3DViewport.TopLeftY = 0.0f;
    Spec.D3DViewport.MinDepth = 0.0f;
    Spec.D3DViewport.MaxDepth = 1.0f;

	Spec.D3DDeviceContext->RSSetViewports(1, &Spec.D3DViewport);

	D3D11_BLEND_DESC blendStateDescription = {};
	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
    //blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	Result = Spec.D3DDevice->CreateBlendState(&blendStateDescription, &Spec.AlphaEnableBlendingState);

	CHECK(Result == S_OK);
	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	Result = Spec.D3DDevice->CreateBlendState(&blendStateDescription, &Spec.AlphaDisableBlendingState);

	CHECK(Result == S_OK);

	//Setup shaders
	BasePassProgram.Init(Spec.D3DDevice, SwapChainBufferSize.x, SwapChainBufferSize.y);
	LightSourceShaderProgram.Init(Spec.D3DDevice);
	BloomTresholdAndDownsampleProgram.Init(Spec.D3DDevice, SwapChainBufferSize.x, SwapChainBufferSize.y, BasePassProgram.GetRenderTargetToDraw().ShaderResourceView);
	BloomQuadCompositeProgram.Init(Spec.D3DDevice,SwapChainBufferSize.x, SwapChainBufferSize.y);
	GaussianBlurShaderProgram.Init(Spec.D3DDevice, SwapChainBufferSize.x, SwapChainBufferSize.y);
	LensFlaresShaderPrograms[0].Init(Spec.D3DDevice, SwapChainBufferSize.x, SwapChainBufferSize.y);
	LensFlaresShaderPrograms[1].Init(Spec.D3DDevice, SwapChainBufferSize.x, SwapChainBufferSize.y);

	//Setup models
	MainScene.InitTestScene(Spec.D3DDevice);

	//Setup matrices
	ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH((float)XM_PI / 4.0f, Spec.D3DViewport.Width / Spec.D3DViewport.Height, SCREEN_NEAR, SCREEN_DEPTH);

	MainScene.Camera.SetPosition(0.f, 0.f, -10.f);
}

void DXRenderer::BeginRendering()
{
	float ClearColorRGBA[4] = { 0.f, 0.f, 0.f, 1.f };

	// Clear the back buffer.
	Spec.D3DDeviceContext->ClearRenderTargetView(Spec.D3DRenderTargetView, ClearColorRGBA);

	// Clear the depth buffer.
	Spec.D3DDeviceContext->ClearDepthStencilView(Spec.D3DDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Clear geom color buffer
	ID3D11RenderTargetView* BasePassColor = BasePassProgram.GetRenderTargetToDraw().RenderTargetView;
	Spec.D3DDeviceContext->ClearRenderTargetView(BasePassColor, ClearColorRGBA);

	// Clear BloomTresholdAndDownsample UAV.
	Spec.D3DDeviceContext->ClearUnorderedAccessViewFloat(BloomTresholdAndDownsampleProgram.GetUAV(), ClearColorRGBA);
}

void DXRenderer::EndRendering()
{
#if ENABLE_VSYNC
	Spec.D3DSwapChain->Present(1, 0);
#else
	Spec.D3DSwapChain->Present(0, 0);
#endif //ENABLE_VSYNC
}

void DXRenderer::RenderFrame(float InDelta)
{
	CameraController MainCameraController(&MainScene.Camera);
	MainCameraController.Update(InDelta);

	MainScene.Camera.UpdateViewMatrix(); 

	BeginRendering();

	ID3D11RenderTargetView* GeomBackBuffer = BasePassProgram.GetRenderTargetToDraw().RenderTargetView;
	Spec.D3DDeviceContext->OMSetRenderTargets(1, &GeomBackBuffer, Spec.D3DDepthStencilView);
	Spec.D3DDeviceContext->OMSetDepthStencilState(Spec.D3DDepthStencilState, 1);

	RenderTeapot();
	RenderTable();

	RenderLightSource();
	// unbind render target and turn depth test off
	Spec.D3DDeviceContext->OMSetRenderTargets(1, &NULL_RT, nullptr);
	Spec.D3DDeviceContext->OMSetDepthStencilState(Spec.D3DNoDepthStencilState, 0);

	RenderBloom();

	//RenderLensFlares();

	EndRendering();
}

void DXRenderer::RenderBloom()
{
	ID3D11RenderTargetView* ThreasholdRenderTargetView = BloomTresholdAndDownsampleProgram.GetRTV();
	Spec.D3DDeviceContext->OMSetRenderTargets(1, &ThreasholdRenderTargetView, nullptr);
	BloomTresholdAndDownsampleProgram.SetShaderParameters(Spec.D3DDeviceContext, 0.3f, BasePassProgram.GetRenderTargetToDraw().ShaderResourceView);
	BloomTresholdAndDownsampleProgram.Draw(Spec.D3DDeviceContext);
	Spec.D3DDeviceContext->PSSetShaderResources(0, 1, &NULL_SRV);

	Spec.D3DDeviceContext->OMSetRenderTargets(1, &NULL_RT, nullptr);

	GaussianBlurShaderProgram.SetShaderParams(Spec.D3DDeviceContext);
	GaussianBlurShaderProgram.Draw(Spec.D3DDeviceContext, BloomTresholdAndDownsampleProgram.GetSRV(), GaussianBlurShaderProgram.GetRenderTargetFinal().RenderTargetView);

	
	ID3D11RenderTargetView* BloomQuadCompositeRenderTargetView = BloomQuadCompositeProgram.GetRenderTarget().RenderTargetView;
	Spec.D3DDeviceContext->OMSetRenderTargets(1, &Spec.D3DRenderTargetView, nullptr);
	BloomQuadCompositeProgram.SetShaderParameters(Spec.D3DDeviceContext, BasePassProgram.GetRenderTargetToDraw().ShaderResourceView, GaussianBlurShaderProgram.GetRenderTargetFinal().ShaderResourceView);
	BloomQuadCompositeProgram.Draw(Spec.D3DDeviceContext);
	ID3D11ShaderResourceView* ShaderResourceViews[] =
	{
		NULL_SRV,
		NULL_SRV
	};

	Spec.D3DDeviceContext->OMSetRenderTargets(1, &NULL_RT, nullptr);
    Spec.D3DDeviceContext->PSSetShaderResources(0, 2, ShaderResourceViews);
}


void DXRenderer::RenderLensFlares()
{
	Spec.TurnOnAlphaBlending();
	Texture2D Lens1;
	Lens1.Initialize(Spec.D3DDevice, L"Resources/Lens-Flare-PNG-Picture.png");
	Texture2D Lens2;
	Lens2.Initialize(Spec.D3DDevice, L"Resources/Lens2.png");
	LensFlaresShaderPrograms[0].SetShaderParams(Spec.D3DDeviceContext, Spec.D3DDevice, Lens1, 0, 0, 0.2, BloomQuadCompositeProgram.GetRenderTarget().ShaderResourceView, Spec.D3DViewport.Width / Spec.D3DViewport.Height);
	LensFlaresShaderPrograms[0].Draw(Spec.D3DDeviceContext, Spec.D3DRenderTargetView);
	Spec.TurnOffAlphaBlending();
}

void DXRenderer::RenderTeapot()
{
	Model & Teapot = MainScene.SceneObjects[MainScene.TeapotIndex];

	XMMATRIX TableWorldMatrix = Teapot.WorldMatrix;
	XMMATRIX ViewMatrix = MainScene.Camera.GetViewMatrix();

	Teapot.ApplyBuffers(Spec.D3DDeviceContext);
	MainScene.PointLight.LightBufferData.ViewPosition = MainScene.Camera.GetPosition();

	BasePassProgram.SetShaderParameters(Spec.D3DDeviceContext, TableWorldMatrix, ViewMatrix, ProjectionMatrix, Teapot, MainScene.PointLight);

	BasePassProgram.Draw(Spec.D3DDeviceContext, Teapot);
}

void DXRenderer::RenderTable()
{
	Model & Table = MainScene.SceneObjects[MainScene.TableIndex];

	XMMATRIX TableWorldMatrix = Table.WorldMatrix;
	XMMATRIX ViewMatrix = MainScene.Camera.GetViewMatrix();

	Table.ApplyBuffers(Spec.D3DDeviceContext);
	MainScene.PointLight.LightBufferData.ViewPosition = MainScene.Camera.GetPosition();

	BasePassProgram.SetShaderParameters(Spec.D3DDeviceContext, TableWorldMatrix, ViewMatrix, ProjectionMatrix, Table, MainScene.PointLight);

	BasePassProgram.Draw(Spec.D3DDeviceContext, Table);
}

void DXRenderer::RenderLightSource()
{
	Model & LightSource = MainScene.SceneObjects[MainScene.LightSourceIndex];

	XMMATRIX LightSourceWorldMatrix = LightSource.WorldMatrix;
	XMMATRIX ViewMatrix = MainScene.Camera.GetViewMatrix();

	LightSource.ApplyBuffers(Spec.D3DDeviceContext);

	LightSourceShaderProgram.SetShaderParameters(Spec.D3DDeviceContext, LightSourceWorldMatrix, ViewMatrix, ProjectionMatrix, LightSource);
	LightSourceShaderProgram.Draw(Spec.D3DDeviceContext, LightSource);
}

