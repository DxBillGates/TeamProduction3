#include "SquareManager.h"

//width,depthは枚数,size.x.zは一辺の大きさ、sizeを変更する場合に敵の生成処理(範囲外チェック)も変更する必要があります
SquareManager::SquareManager():width(16),depth(16),size(Vector3((float)128,1, (float)128))
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
			pos.x = i * size.x + size.x/2.f;
			pos.z = j * size.z + size.z/2.f;
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
	Vector3 red = Vector3((float)245 / 255, (float)27 / 255, (float)27 / 255), blue = Vector3(0, 0, 1), white = Vector3(1, 1, 1);
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

Vector3 SquareManager::GetSize()
{
	Vector3 s;
	s.x = width * size.x;
	s.y = 0;
	s.z = depth * size.z;
	return s;
}
