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
	while (std::getline(file, line))    //�t�@�C������1�s�Âǂݍ���
	{
		std::istringstream line_stream(line);    //getline�œǂݍ��߂�悤��string����stream�ɕϊ�����
		std::string key;
		std::getline(line_stream, key, ' ');    //line_stream����󔒕������o�͂����܂�key�ɓ��͂���
		if (key == "mtllib")
		{
			std::string materialFilename;
			line_stream >> materialFilename;
			LoadMaterialData(directoryPath, materialFilename, outputMeshData.materialData);
		}
		if (key == "v")    //���_���W�̓ǂݍ���
		{
			DirectX::XMFLOAT3 pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
			//positions.push_back(pos);    //�P�Ȃ�float�ȂǂȂ�push_back�ł�OK
			positions.emplace_back(pos);    //�N���X��\���̂̏ꍇemplace_back�̂ق�������
		}
		if (key == "vt")    //�e�N�X�`�����W�̓ǂݍ���
		{
			DirectX::XMFLOAT2 uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uvies.emplace_back(uv);
		}
		if (key == "vn")    //�@�����̓ǂݍ���
		{
			DirectX::XMFLOAT3 normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			normals.emplace_back(normal);
		}
		if (key == "f")    //v,vt,vn�̑g�ݍ��킹�𐶐�
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

			//���_����g�ݍ��킹��
			for (int i = 0; i < (int)localIndices.size(); i += 3)
			{
				int v, vt, vn;
				v = localIndices[i];
				vt = localIndices[i + 1];
				vn = localIndices[i + 2];
				outputMeshData.vertices.emplace_back(positions[v], uvies[vt], normals[vn]);
			}
			//�ʏ�񂩂�C���f�b�N�X�����
			//4���_��3���_�ŕ���
			//4���_�Ȃ�6�ǉ�
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
			//3���_�Ȃ�3�ǉ�
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
		std::istringstream line_stream(line);    //getline�œǂݍ��߂�悤��string����stream�ɕϊ�����
		std::string key;
		std::getline(line_stream, key, ' ');    //line_stream����󔒕������o�͂����܂�key�ɓ��͂���
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}
		if (key == "newmtl")    //�}�e���A���̖��O
		{
			line_stream >> outputMaterialData.name;
		}
		if (key == "Ka")    //�A���r�G���g
		{
			line_stream >> outputMaterialData.material.ambient.x;
			line_stream >> outputMaterialData.material.ambient.y;
			line_stream >> outputMaterialData.material.ambient.z;
			outputMaterialData.material.ambient.w = 0;
		}
		if (key == "Kd")    //�f�B�t���[�Y
		{
			line_stream >> outputMaterialData.material.diffuse.x;
			line_stream >> outputMaterialData.material.diffuse.y;
			line_stream >> outputMaterialData.material.diffuse.z;
			outputMaterialData.material.diffuse.w = 0;
		}
		if (key == "Ks")    //�X�y�L�����[
		{
			line_stream >> outputMaterialData.material.specular.x;
			line_stream >> outputMaterialData.material.specular.y;
			line_stream >> outputMaterialData.material.specular.z;
			outputMaterialData.material.specular.w = 0;
		}
		if (key == "map_Kd")    //�e�N�X�`��
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

void LoadContents::CreateModelData_Plane(float width, float height, SpriteData & outputSpriteData)
{
	outputSpriteData.vertices =
	{
		{{    0,     0,0},{0,0}},
		{{width,     0,0},{1,0}},
		{{width,height,0},{1,1}},
		{{    0,height,0},{0,1}},
	};
	outputSpriteData.indices =
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
