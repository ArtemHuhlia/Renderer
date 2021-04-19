#pragma once
#include <string>
#include <d3d11.h>

class Shader
{
protected:
	std::wstring FileName;
	std::string EntryPoint;
	ID3DBlob* Binaries;
public:
	Shader() 
	{

	}

	Shader(std::wstring InFileName, std::string InEntryPoint) :
		FileName(InFileName),
		EntryPoint(InEntryPoint)
	{

	};
	virtual void CompileShader() = 0;
	virtual void CreateShader(ID3D11Device* InDevice) = 0;
	ID3DBlob* GetBinaries() const 
	{
		return Binaries;
	}
};

class PixelShader : public Shader
{
	ID3D11PixelShader* D3DPixelShader;
public:
	PixelShader()
	{

	};
	PixelShader(std::wstring InFileName, std::string InEntryPoint) :
		Shader(InFileName, InEntryPoint)
	{

	};

	virtual void CompileShader()	override;
	virtual void CreateShader(ID3D11Device* InDevice)		override;
	ID3D11PixelShader* GetD3DPixelShader()
	{
		return D3DPixelShader;
	}

};

class VertexShader : public Shader
{
	ID3D11VertexShader* D3DVertexShader;
public:
	VertexShader()
	{

	};
	VertexShader(std::wstring InFileName, std::string InEntryPoint) :
		Shader(InFileName, InEntryPoint)
	{

	};

	virtual void CompileShader()	override;
	virtual void CreateShader(ID3D11Device* InDevice)		override;
	ID3D11VertexShader* GetD3DVertexShader()
	{
		return D3DVertexShader;
	}
};
