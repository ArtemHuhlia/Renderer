#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

class Texture2D
{
	ID3D11ShaderResourceView * SRV;
	ID3D11Texture2D	* D3DTexture2D;
	DirectX::XMINT3 Size;
	int32_t BytesPerPixel;
public:

	ID3D11ShaderResourceView * GetTexture()
	{
		return SRV;
	}


	void Initialize(ID3D11Device * InDevice, std::wstring InFilename);
};
