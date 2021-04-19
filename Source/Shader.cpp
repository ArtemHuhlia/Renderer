#include "Shader.h"
#include "d3dcompiler.h"
#include "CoreMinimal.h"

void PixelShader::CompileShader()
{
	CHECK(!FileName.empty());
	CHECK(!EntryPoint.empty());

	UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( RE_DEBUG )
	Flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif
	//Blob: error massage
	ID3DBlob * ErrorMessage = nullptr;

	const HRESULT Result = D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), "ps_5_0", Flags, 0u, &Binaries, &ErrorMessage);
	if (FAILED(Result))
	{
		if (ErrorMessage)
		{
			OutputDebugStringA(reinterpret_cast<char *>(ErrorMessage->GetBufferPointer()));
		}
		if(Binaries)
		{
			Binaries->Release();
		}
		CHECK(false); // look at the console. smth goes wrong
	}
}

void VertexShader::CompileShader()
{
	CHECK(!FileName.empty());
	CHECK(!EntryPoint.empty());

	UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( RE_DEBUG )
	Flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif
	//Blob: error massage
	ID3DBlob * ErrorMessage = nullptr;

	const HRESULT Result = D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), "vs_5_0", Flags, 0u, &Binaries, &ErrorMessage);
	if (FAILED(Result))
	{
		if (ErrorMessage)
		{
			OutputDebugStringA(reinterpret_cast<char *>(ErrorMessage->GetBufferPointer()));
		}
		if(Binaries)
		{
			Binaries->Release();
		}
		CHECK(false); // look at the console. smth goes wrong
	}
}

void PixelShader::CreateShader(ID3D11Device* InDevice)
{
	// We have to compile shader before
	CHECK(Binaries);
	CHECK(InDevice);

	const HRESULT Result = InDevice->CreatePixelShader(Binaries->GetBufferPointer(), Binaries->GetBufferSize(), nullptr, &D3DPixelShader);
	CHECK(SUCCEEDED(Result));
}

void VertexShader::CreateShader(ID3D11Device* InDevice)
{
	// We have to compile shader before
	CHECK(Binaries);
	CHECK(InDevice);

	const HRESULT Result = InDevice->CreateVertexShader(Binaries->GetBufferPointer(), Binaries->GetBufferSize(), nullptr, &D3DVertexShader);
	CHECK(SUCCEEDED(Result));
}
