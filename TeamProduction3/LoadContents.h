#pragma once
#include"Dx12_Device.h"
#include"Dx12_VertexData.h"
#include"Dx12_Texture2D.h"
#include<DirectXMath.h>
#include<vector>
#include<string>
struct Material
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
};
struct MaterialData
{
	std::string name;
	std::string textureName;
	Dx12_Texture2D* texture = nullptr;
	Material material;
	~MaterialData()
	{
		delete texture;
	}
};
struct MeshData
{
	std::string modelName;
	std::vector<VertexUV_Normal> vertices;
	std::vector<unsigned short> indices;
	MaterialData materialData;
};
struct SpriteData
{
	std::vector<VertexUV> vertices;
	std::vector<unsigned short> indices;
};
class LoadContents
{
private:
	Dx12_Device* device;
	Dx12_CBVSRVUAVHeap* heap;
public:
	LoadContents(Dx12_Device* device, Dx12_CBVSRVUAVHeap* heap);
	~LoadContents();
	void LoadMeshData(std::string directoryPath, std::string modelName, MeshData& outputMeshData);
	void LoadMaterialData(std::string directoryPath, std::string fileName, MaterialData& outputMaterialData);
	void CreateModelData_Plane(float width,float height,MeshData& outputMeshData);
	void CreateModelData_Plane(float width,float height,SpriteData& outputSpriteData,bool centerFlag = 0);
	void CreateModelData_Floor(float width,float depth, MeshData& outputMeshData);
	Dx12_Texture2D* LoadTexture(std::string directoryPath, std::string fileName);
};

