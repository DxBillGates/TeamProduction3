#pragma once
#include "Square.h"

class SquareManager
{
private:
	int width;
	int depth;
	int redTile;
	int blueTile;
	int whiteTile;
	Vector3 size;
	std::vector<std::vector<Square>> squares;
public:
	SquareManager();
	~SquareManager();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	std::vector<std::vector<Square>>* GetSquares();
	Vector3 GetTilesInfomation();
	int GetWidth();
	int GetDepth();
	Vector3 GetSize();
};
