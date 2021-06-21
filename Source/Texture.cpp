#include "Texture.h"
#include "CoreMinimal.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ThirdParty/WICTextureLoader11.h"

void Texture2D::Initialize(ID3D11Device * InDevice, std::wstring InFilename)
{
	//stbi_set_flip_vertically_on_load(1); //Do we need that? since its not opengl, should be checked
	//void * TextureData = stbi_load(InFilename.c_str(), &Size.x, &Size.y, &BytesPerPixel, 0);
	ID3D11Resource * TexRes = D3DTexture2D;

	HRESULT Result = DirectX::CreateWICTextureFromFile(InDevice, InFilename.c_str(), &TexRes, &SRV);
	CHECK(SUCCEEDED(Result));
	/*D3D11_TEXTURE2D_DESC TextureDesc = {};

	TextureDesc.Width  = Size.x;
    TextureDesc.Height = Size.y;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA TextureSubresData = {};
    TextureSubresData.pSysMem        = TextureData;
    TextureSubresData.SysMemPitch    = Size.x * Size.y * BytesPerPixel;

	Result  = InDevice->CreateTexture2D(&TextureDesc, &TextureSubresData, &D3DTexture2D);
	CHECK(SUCCEEDED(Result));
	Result = InDevice->CreateShaderResourceView(D3DTexture2D, nullptr, &SRV);
	CHECK(SUCCEEDED(Result));*/
}

