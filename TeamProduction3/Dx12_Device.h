#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
class Dx12_Device
{
	D3D12_VIEWPORT               mViewport = {};
	D3D12_RECT                   mRect = {};
	IDXGIFactory6*               mDxgiFactory = nullptr;
	std::vector<IDXGIAdapter*>   mEnumAdapter;
	IDXGIAdapter*                mUseAdapter = nullptr;
	ID3D12Device*                mDevice = nullptr;
	ID3D12CommandAllocator*      mCmdAlloc = nullptr;
	ID3D12GraphicsCommandList*   mCmdList = nullptr;
	ID3D12CommandQueue*          mCmdQueue = nullptr;
	IDXGISwapChain4*             mSwapChain = nullptr;
	std::vector<ID3D12Resource*> mFrameBuffer;
	ID3D12DescriptorHeap*        mRtvHeap = nullptr;
	ID3D12DescriptorHeap*        mDsvHeap = nullptr;
	ID3D12Resource*              mDepthBuffer = nullptr;
	ID3D12Fence*                 mFence = nullptr;
	UINT64                       mFenceValue = 0;
private:
	void SetViewport(int wndWidth, int wndHeight);
	void CreateDxgiFactory();
	void CreateDevice();
	void CreateCmdList();
	void CreateSwapChain(HWND hwnd);
	void CreateRtv();
	void CreateDsv();
	void CreateFence();
	void SetResourceBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
public:
	Dx12_Device(HWND hwnd, int wndWidth, int wndHeight);
	~Dx12_Device();
	void ClearScreen(DirectX::XMFLOAT4 color);
	void ClearDepth();
	void ScreecFlip();
	void CommandReset();
	ID3D12Device* GetDevice();
	ID3D12GraphicsCommandList* GetCmdList();
};

