#include "LoadContents.h"
#include <fstream>
#include <sstream>
#pragma comment(lib,"winmm.lib")

LoadContents::LoadContents(Dx12_Device * device, Dx12_CBVSRVUAVHeap * heap)
	: device(device), heap(heap)
{
}

LoadContents::~LoadContents()
{
}

void LoadContents::LoadMeshData(std::string directoryPath, std::string modelName, MeshData & outputMeshData)
{
	outputMeshData.modelName = modelName;
	std::string  filepath = directoryPath + modelName + ".obj";
	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT2> uvies;
	std::vector<DirectX::XMFLOAT3> normals;
	int startIndex = 0;

	std::ifstream file;
	file.open(filepath);
	if (file.fail())return;
	std::string line;
	while (std::getline(file, line))    //ファイルから1行づつ読み込む
	{
		std::istringstream line_stream(line);    //getlineで読み込めるようにstringからstreamに変換する
		std::string key;
		std::getline(line_stream, key, ' ');    //line_streamから空白文字が出力されるまでkeyに入力する
		if (key == "mtllib")
		{
			std::string materialFilename;
			line_stream >> materialFilename;
			LoadMaterialData(directoryPath, materialFilename, outputMeshData.materialData);
		}
		if (key == "v")    //頂点座標の読み込み
		{
			DirectX::XMFLOAT3 pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
			//positions.push_back(pos);    //単なるfloatなどならpush_backでもOK
			positions.emplace_back(pos);    //クラスや構造体の場合emplace_backのほうが早い
		}
		if (key == "vt")    //テクスチャ座標の読み込み
		{
			DirectX::XMFLOAT2 uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uvies.emplace_back(uv);
		}
		if (key == "vn")    //法線情報の読み込み
		{
			DirectX::XMFLOAT3 normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			normals.emplace_back(normal);
		}
		if (key == "f")    //v,vt,vnの組み合わせを生成
		{
			std::string index_word;
			std::string index;
			int number;
			std::vector<int> localIndices;
			int vCount = 0;
			while (std::getline(line_stream, index_word, ' '))
			{
				std::istringstream indexWord_stream(index_word);
				while (std::getline(indexWord_stream, index, '/'))
				{
					std::istringstream index_stream(index);
					index_stream >> number;
					localIndices.push_back(number - 1);
				}
				++vCount;
			}

			//頂点情報を組み合わせる
			for (int i = 0; i < (int)localIndices.size(); i += 3)
			{
				int v, vt, vn;
				v = localIndices[i];
				vt = localIndices[i + 1];
				vn = localIndices[i + 2];
				outputMeshData.vertices.emplace_back(positions[v], uvies[vt], normals[vn]);
			}
			//面情報からインデックスを作る
			//4頂点と3頂点で分岐
			//4頂点なら6個追加
			if (vCount == 4)
			{
				outputMeshData.indices.push_back(startIndex);
				outputMeshData.indices.push_back(startIndex + 1);
				outputMeshData.indices.push_back(startIndex + 2);
				outputMeshData.indices.push_back(startIndex + 2);
				outputMeshData.indices.push_back(startIndex + 3);
				outputMeshData.indices.push_back(startIndex);
				startIndex += vCount;
			}
			//3頂点なら3個追加
			if (vCount == 3)
			{
				outputMeshData.indices.push_back(startIndex);
				outputMeshData.indices.push_back(startIndex + 1);
				outputMeshData.indices.push_back(startIndex + 2);
				startIndex += vCount;
			}
		}
	}
	file.close();
}

void LoadContents::LoadMaterialData(std::string directoryPath, std::string fileName, MaterialData & outputMaterialData)
{
	std::ifstream file;
	std::string filepath = directoryPath + fileName;
	file.open(filepath);
	if (file.fail())return;
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream line_stream(line);    //getlineで読み込めるようにstringからstreamに変換する
		std::string key;
		std::getline(line_stream, key, ' ');    //line_streamから空白文字が出力されるまでkeyに入力する
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}
		if (key == "newmtl")    //マテリアルの名前
		{
			line_stream >> outputMaterialData.name;
		}
		if (key == "Ka")    //アンビエント
		{
			line_stream >> outputMaterialData.material.ambient.x;
			line_stream >> outputMaterialData.material.ambient.y;
			line_stream >> outputMaterialData.material.ambient.z;
			outputMaterialData.material.ambient.w = 0;
		}
		if (key == "Kd")    //ディフューズ
		{
			line_stream >> outputMaterialData.material.diffuse.x;
			line_stream >> outputMaterialData.material.diffuse.y;
			line_stream >> outputMaterialData.material.diffuse.z;
			outputMaterialData.material.diffuse.w = 0;
		}
		if (key == "Ks")    //スペキュラー
		{
			line_stream >> outputMaterialData.material.specular.x;
			line_stream >> outputMaterialData.material.specular.y;
			line_stream >> outputMaterialData.material.specular.z;
			outputMaterialData.material.specular.w = 0;
		}
		if (key == "map_Kd")    //テクスチャ
		{
			line_stream >> outputMaterialData.textureName;
			outputMaterialData.texture = LoadTexture(directoryPath, outputMaterialData.textureName);
		}
	}
	file.close();
}

void LoadContents::CreateModelData_Plane(float width, float height, MeshData & outputMeshData)
{
	outputMeshData.modelName = "Plane";
	outputMeshData.materialData.material = {};
	outputMeshData.materialData.name = "none";
	outputMeshData.materialData.texture = nullptr;
	outputMeshData.materialData.textureName = "none";
	outputMeshData.vertices =
	{
		{{-width / 2.0f, height / 2.0f,0},{0,0},{0,0,-1}},
		{{ width / 2.0f, height / 2.0f,0},{1,0},{0,0,-1}},
		{{ width / 2.0f,-height / 2.0f,0},{1,1},{0,0,-1}},
		{{-width / 2.0f,-height / 2.0f,0},{0,1},{0,0,-1}},
	};
	outputMeshData.indices =
	{
		0,1,2,
		0,2,3,
	};
}

void LoadContents::CreateModelData_Plane(float width, float height, SpriteData & outputSpriteData, bool centerFlag)
{
	if (!centerFlag)
	{
		outputSpriteData.vertices =
		{
			{{    0,     0,0},{0,0}},
			{{width,     0,0},{1,0}},
			{{width,height,0},{1,1}},
			{{    0,height,0},{0,1}},
		};
	}
	else
	{
		outputSpriteData.vertices =
		{
			{{-width/2.0f,-height/2.0f,0},{0,0}},
			{{ width/2.0f,-height/2.0f,0},{1,0}},
			{{ width/2.0f,height/2.0f,0},{1,1}},
			{{-width/2.0f,height/2.0f,0},{0,1}},
		};
	}
	outputSpriteData.indices =
	{
		0,1,2,
		0,2,3,
	};
}

void LoadContents::CreateModelData_Floor(float width, float depth, MeshData & outputMeshData)
{
	outputMeshData.modelName = "Plane";
	outputMeshData.materialData.material = {};
	outputMeshData.materialData.name = "none";
	outputMeshData.materialData.texture = nullptr;
	outputMeshData.materialData.textureName = "none";
	outputMeshData.vertices =
	{
		{{-width / 2.0f,0, depth / 2.0f},{0,0},{0,1,0}},
		{{ width / 2.0f,0, depth / 2.0f},{1,0},{0,1,0}},
		{{ width / 2.0f,0,-depth / 2.0f},{1,1},{0,1,0}},
		{{-width / 2.0f,0,-depth / 2.0f},{0,1},{0,1,0}},
	};
	outputMeshData.indices =
	{
		0,1,2,
		0,2,3,
	};
}

Dx12_Texture2D * LoadContents::LoadTexture(std::string directoryPath, std::string fileName)
{
	std::string filepath = directoryPath + fileName;
	wchar_t wFilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wFilepath, _countof(wFilepath));
	return new Dx12_Texture2D(wFilepath, device->GetDevice(), heap);
}
