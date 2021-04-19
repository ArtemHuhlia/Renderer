#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#undef max
#include "tiny_obj_loader.h"
#include "Math/Vector3.h"
#include "CoreMinimal.h"

void Model::LoadFromObj(std::string InFilePath, ID3D11Device* InDevice)
{
	tinyobj::attrib_t Attributes;
	std::vector<tinyobj::material_t> Materials;
	std::vector<tinyobj::shape_t> Shapes;
	std::string ErrorMessage;
	std::string WarningMessage;
	bool Result = tinyobj::LoadObj(&Attributes, &Shapes, &Materials, &WarningMessage, &ErrorMessage, InFilePath.c_str());
	if (!ErrorMessage.empty())
	{
		LOG(("%s"), ErrorMessage.c_str());
	}

	if (!WarningMessage.empty())
	{
		LOG(("%s"), WarningMessage.c_str());
	}

	// We quite stricked now 
	CHECK(Result);
	CHECK(Shapes.size() == 1);

	std::vector<tinyobj::index_t> & Indices = Shapes[0].mesh.indices;

	for (int i = 0; i < Indices.size(); i += 3) // triangles iterate 
	{
		const size_t FirstIndex = Indices[i].vertex_index * 3;
		const size_t SecondIndex = Indices[i + 1].vertex_index * 3;
		const size_t ThirdIndex = Indices[i + 2].vertex_index * 3;

		//Positions
		const Vector3f Position1 = Vector3f(Attributes.vertices[FirstIndex], Attributes.vertices[FirstIndex + 1], Attributes.vertices[FirstIndex + 2]);
		const Vector3f Position2 = Vector3f(Attributes.vertices[SecondIndex], Attributes.vertices[SecondIndex + 1], Attributes.vertices[SecondIndex + 2]);
		const Vector3f Position3 = Vector3f(Attributes.vertices[ThirdIndex], Attributes.vertices[ThirdIndex + 1], Attributes.vertices[ThirdIndex + 2]);

		//Normals
		Vector3f Normal1;
		Vector3f Normal2;
		Vector3f Normal3;

		//calculate normals if they are not present
		if (Attributes.normals.empty() ||
			(Indices[i].normal_index < 0) ||
			(Indices[i + 1].normal_index < 0) ||
			(Indices[i + 2].normal_index < 0))
		{
			const Vector3f EdgeVector1 = Position3 - Position2;
			const Vector3f EdgeVector2 = Position1 - Position2;

			Vector3f GeneralNormal = EdgeVector1.Cross(EdgeVector2);
			GeneralNormal.Normalize();
			Normal1 = GeneralNormal;
			Normal2 = GeneralNormal;
			Normal3 = GeneralNormal;
		}
		else
		{
			const size_t FirstIndexN = Indices[i].normal_index * 3;
			const size_t SecondIndexN = Indices[i + 1].normal_index * 3;
			const size_t ThirdIndexN = Indices[i + 2].normal_index * 3;

			Normal1 = Vector3f(Attributes.normals[FirstIndexN], Attributes.normals[FirstIndexN + 1], Attributes.normals[FirstIndexN + 2]);
			Normal2 = Vector3f(Attributes.normals[SecondIndexN], Attributes.normals[SecondIndexN + 1], Attributes.normals[SecondIndexN + 2]);
			Normal3 = Vector3f(Attributes.normals[ThirdIndexN], Attributes.normals[ThirdIndexN + 1], Attributes.normals[ThirdIndexN + 2]);
		}

		//Texture coords
		XMFLOAT2 TextureCoord1; 
		XMFLOAT2 TextureCoord2;
		XMFLOAT2 TextureCoord3;

		if (Attributes.texcoords.empty() ||
			(Indices[i].texcoord_index < 0) ||
			(Indices[i + 1].texcoord_index < 0) ||
			(Indices[i + 2].texcoord_index < 0))
		{
			//TODO: UV should be generated
			TextureCoord1 = XMFLOAT2(0.f, 0.f);
			TextureCoord2 = XMFLOAT2(0.f, 0.f);
		}
		else
		{

			const size_t FirstIndexUV = Indices[i].texcoord_index * 2;
			const size_t SecondIndexUV = Indices[i + 1].texcoord_index * 2;
			const size_t ThirdIndexUV = Indices[i + 2].texcoord_index * 2;

			TextureCoord1 = XMFLOAT2(Attributes.texcoords[FirstIndexUV], Attributes.texcoords[FirstIndexUV + 1]);
			TextureCoord2 = XMFLOAT2(Attributes.texcoords[SecondIndexUV], Attributes.texcoords[SecondIndexUV + 1]);
			TextureCoord3 = XMFLOAT2(Attributes.texcoords[ThirdIndexUV], Attributes.texcoords[ThirdIndexUV + 1]);

		}
		
		VertexDataType Vertex1;
		Vertex1.Position.x = Position1.X;
		Vertex1.Position.y = Position1.Y;
		Vertex1.Position.z = Position1.Z;

		Vertex1.Normal.x = Normal1.X;
		Vertex1.Normal.y = Normal1.Y;
		Vertex1.Normal.z = Normal1.Z;

		Vertex1.UV = TextureCoord1;

		VertexDataType Vertex2;

		Vertex2.Position.x = Position2.X;
		Vertex2.Position.y = Position2.Y;
		Vertex2.Position.z = Position2.Z;

		Vertex2.Normal.x = Normal2.X;
		Vertex2.Normal.y = Normal2.Y;
		Vertex2.Normal.z = Normal2.Z;

		Vertex2.UV = TextureCoord2;

		VertexDataType Vertex3;

		Vertex3.Position.x = Position3.X;
		Vertex3.Position.y = Position3.Y;
		Vertex3.Position.z = Position3.Z;

		Vertex3.Normal.x = Normal3.X;
		Vertex3.Normal.y = Normal3.Y;
		Vertex3.Normal.z = Normal3.Z;

		Vertex3.UV = TextureCoord3;

		VertexData.push_back(Vertex1);
		VertexData.push_back(Vertex2);
		VertexData.push_back(Vertex3);

	}

	Initialize(InDevice);
}


void Model::Initialize(ID3D11Device* InDevice)
{
	CHECK(InDevice);
	CHECK(VertexData.size() > 0);
	D3D11_BUFFER_DESC VertexBufferDesc;

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexDataType) * VertexData.size();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexSubresData;

	VertexSubresData.pSysMem = VertexData.data();
	VertexSubresData.SysMemPitch = 0;
	VertexSubresData.SysMemSlicePitch = 0;

	const HRESULT Result = InDevice->CreateBuffer(&VertexBufferDesc, &VertexSubresData, &VertexBuffer);
	CHECK(SUCCEEDED(Result));
}

void Model::Release()
{
	if (VertexBuffer)
	{
		VertexBuffer->Release();
		VertexBuffer = nullptr;
	}
}



void Model::ApplyBuffers(ID3D11DeviceContext* InDeviceContext)
{
	CHECK(InDeviceContext);

	const uint32_t VertexBufferStride = sizeof(VertexDataType);
	const uint32_t VertexBufferOffset = 0u;

	InDeviceContext->IASetVertexBuffers(0u, 1u, &VertexBuffer, &VertexBufferStride, &VertexBufferOffset);

	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
