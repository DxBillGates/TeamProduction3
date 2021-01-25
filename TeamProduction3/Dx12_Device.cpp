#include "Dx12_Device.h"
#include "COMRelease.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
void Dx12_Device::SetViewport(int wndWidth, int wndHeight)
{
	mViewport = { 0,0,(float)wndWidth,(float)wndHeight,0,1.0f };
	mRect = { 0,0,wndWidth,wndHeight };
}

void Dx12_Device::CreateDxgiFactory()
{
	//DXGIFactoryの生成
	HRESULT result;
	result = CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory));
}

void Dx12_Device::CreateDevice()
{
	//使用できるGPUを探してそのアダプタでID3DDeviceを生成
	HRESULT result;
	for (int i = 0; mDxgiFactory->EnumAdapters(i, &mUseAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		mEnumAdapter.push_back(mUseAdapter);
	}
	DXGI_ADAPTER_DESC adapterDesc = {};
	for (int i = 0; i < (int)mEnumAdapter.size(); ++i)
	{
		mEnumAdapter[i]->GetDesc(&adapterDesc);
		std::wstring adapterName = adapterDesc.Description;
		if (adapterName.find(L"Microsoft") == std::wstring::npos && adapterName.find(L"Intel") == std::wstring::npos)
		{
			mUseAdapter = mEnumAdapter[i];
			break;
		}
	}
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	for (int i = 0; i < _countof(featureLevel); ++i)
	{
		if (SUCCEEDED(result = D3D12CreateDevice(mUseAdapter, featureLevel[i], IID_PPV_ARGS(&mDevice))))break;
	}
}

void Dx12_Device::CreateCmdList()
{
	//コマンド関係を生成
	HRESULT result;
	result = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCmdAlloc));
	result = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAlloc, nullptr, IID_PPV_ARGS(&mCmdList));
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	result = mDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue));
}

void Dx12_Device::CreateSwapChain(HWND hwnd)
{
	//ApplicationからgameWindowのハンドルを取得しそのハンドルとそのほかの設定からSwapChainを生成
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = (UINT)mViewport.Width;
	swapChainDesc.Height = (UINT)mViewport.Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	result = mDxgiFactory->CreateSwapChainForHwnd(mCmdQueue, hwnd, &swapChainDesc, nullptr, nullptr, (IDXGISwapChain1**)&mSwapChain);
}

void Dx12_Device::CreateRtv()
{
	//RTVを格納するためのヒープを生成
	HRESULT result;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	result = mDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap));
	mFrameBuffer.resize(rtvHeapDesc.NumDescriptors);
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//SwapChainからバックバッファ、フロントバッファを取得しそれをもとにRTVを生成
	for (int i = 0; i < (int)mFrameBuffer.size(); ++i)
	{
		result = mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mFrameBuffer[i]));
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHeapHandle.ptr += i * mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDevice->CreateRenderTargetView(mFrameBuffer[i], &rtvDesc, rtvHeapHandle);
	}
}

void Dx12_Device::CreateDsv()
{
	//デプスバッファの設定
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = (UINT64)mViewport.Width;
	resDesc.Height = (UINT64)mViewport.Height;
	resDesc.DepthOrArraySize = 1;
	resDesc.Format = DXGI_FORMAT_D32_FLOAT;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//デプスバッファのフォーマットと最大深度値を設定
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	//設定をもとにデプスバッファを生成
	result = mDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&mDepthBuffer));
	//DSVを格納するためのヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//DSVヒープの生成
	result = mDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap));
	//DSVの生成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = resDesc.Format;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	mDevice->CreateDepthStencilView(mDepthBuffer, &dsvDesc, mDsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void Dx12_Device::CreateFence()
{
	//CPUとGPUの同期をさせる役割を持つフェンスを生成
	HRESULT result;
	result = mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
}

void Dx12_Device::SetResourceBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = mFrameBuffer[mSwapChain->GetCurrentBackBufferIndex()];
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	mCmdList->ResourceBarrier(1, &barrier);
}

Dx12_Device::Dx12_Device(HWND hwnd, int wndWidth, int wndHeight)
{
#ifdef _DEBUG
	HRESULT result;
	ID3D12Debug* debug;
	if (SUCCEEDED(result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		COM_RELEASE(debug);
	}
#endif
	SetViewport(wndWidth, wndHeight);
	CreateDxgiFactory();
	CreateDevice();
	CreateCmdList();
	CreateSwapChain(hwnd);
	CreateRtv();
	CreateDsv();
	CreateFence();
}

Dx12_Device::~Dx12_Device()
{
	mCmdQueue->Signal(mFence, ++mFenceValue);
	if (mFence->GetCompletedValue() != mFenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	COM_RELEASE(mDxgiFactory);
	COM_RELEASE(mDevice);
	COM_RELEASE(mCmdAlloc);
	COM_RELEASE(mCmdList);
	COM_RELEASE(mCmdQueue);
	COM_RELEASE(mFrameBuffer[0]);
	COM_RELEASE(mFrameBuffer[1]);
	COM_RELEASE(mSwapChain);
	COM_RELEASE(mRtvHeap);
	COM_RELEASE(mDepthBuffer);
	COM_RELEASE(mDsvHeap);
	COM_RELEASE(mFence);
}

void Dx12_Device::ClearScreen(DirectX::XMFLOAT4 color)
{
	SetResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	/* 画面クリアコマンド */
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += mSwapChain->GetCurrentBackBufferIndex() * mDevice->GetDescriptorHandleIncrementSize(mRtvHeap->GetDesc().Type);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	mCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	float rgba[] = { color.x,color.y,color.z,color.w };
	mCmdList->ClearRenderTargetView(rtvHandle, rgba, 0, nullptr);
	mCmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	mCmdList->RSSetViewports(1, &mViewport);
	mCmdList->RSSetScissorRects(1, &mRect);
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Dx12_Device::ClearDepth()
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	mCmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void Dx12_Device::ScreecFlip()
{
	SetResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	mCmdList->Close();
	ID3D12CommandList* cmdLists[] = { mCmdList };
	mCmdQueue->ExecuteCommandLists(1, cmdLists);
	mCmdQueue->Signal(mFence, ++mFenceValue);
	if (mFence->GetCompletedValue() != mFenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	mCmdAlloc->Reset();
	mCmdList->Reset(mCmdAlloc, nullptr);
	mSwapChain->Present(0, 0);
}

void Dx12_Device::CommandReset()
{
	mCmdAlloc->Reset();
	mCmdList->Reset(mCmdAlloc, nullptr);
}

ID3D12Device * Dx12_Device::GetDevice()
{
	return mDevice;
}

ID3D12GraphicsCommandList * Dx12_Device::GetCmdList()
{
	return mCmdList;
}
