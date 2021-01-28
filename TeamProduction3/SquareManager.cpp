#include "SquareManager.h"

//width,depth�͖���,size.x.z�͈�ӂ̑傫���Asize��ύX����ꍇ�ɓG�̐�������(�͈͊O�`�F�b�N)���ύX����K�v������܂�
SquareManager::SquareManager():width(32),depth(32),size(Vector3((float)64,1, (float)64))
{
	squares.resize(width);
	for (auto itr = squares.begin(); itr != squares.end(); ++itr)
	{
		itr->resize(depth);
	}
	Square::SetSize(size);
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < depth; ++j)
		{
			Vector3 pos;
			pos.x = i * size.x + (float)width/2.f;
			pos.z = j * size.z + (float)depth/2.f;
			squares[i][j].SetPosition(pos);
		}
	}
}

SquareManager::~SquareManager()
{
}

void SquareManager::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	Square::StaticLoadAsset(device, heap, loader);
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < depth; ++j)
		{
			squares[i][j].LoadAsset(device, heap, loader);
		}
	}
}

void SquareManager::Initialize()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < depth; ++j)
		{
			squares[i][j].Initialize();
		}
	}
}

void SquareManager::Update()
{
	redTile = blueTile = whiteTile = 0;
	Vector3 red = Vector3(1,0,0), blue = Vector3(0, 0, 1), white = Vector3(1, 1, 1);
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < depth; ++j)
		{
			if (squares[i][j].GetColor() == white)whiteTile++;
			else if (squares[i][j].GetColor() == red)redTile++;
			else if (squares[i][j].GetColor() == blue)blueTile++;
			squares[i][j].Update();
		}
	}
}

void SquareManager::Draw(ID3D12GraphicsCommandList * cmdList)
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < depth; ++j)
		{
			squares[i][j].Draw(cmdList);
		}
	}
}

std::vector<std::vector<Square>>* SquareManager::GetSquares()
{
	return &squares;
}

Vector3 SquareManager::GetTilesInfomation()
{
	return Vector3((float)redTile, (float)blueTile, (float)whiteTile);
}

int SquareManager::GetWidth()
{
	return width;
}

int SquareManager::GetDepth()
{
	return depth;
}
