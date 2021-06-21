#pragma once

#include <d3d11.h>
#include "CoreMinimal.h"


struct RenderTarget
{
	ID3D11Texture2D* RenderTargetTexture;
    ID3D11RenderTargetView* RenderTargetView;
    ID3D11ShaderResourceView* ShaderResourceView;
    ID3D11UnorderedAccessView* UnorderedAccessView;
	float Width;
	float Height;
	RenderTarget() :
		RenderTargetTexture(nullptr),
		RenderTargetView(nullptr),
		ShaderResourceView(nullptr),
		UnorderedAccessView(nullptr)
	{}

	void Init(ID3D11Device * InDevice, float InWidth, float InHeight)
	{
		Width = InWidth;
		Height = InHeight;

		D3D11_TEXTURE2D_DESC TextureDesc = {};
		D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc = {};
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};

		// 1. Create Texture2D
		TextureDesc.Width = Width;
		TextureDesc.Height = Height;
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

		// 2. Create render target view
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
	}

};
