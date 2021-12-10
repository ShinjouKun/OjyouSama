#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

class RWStructuredBuffer
{
public:
	RWStructuredBuffer();
	~RWStructuredBuffer();

	void init(int sizeOfelement, int numElement, void* initData);

	//UAVに登録
	void RegistUnorderAccessView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

	//初期化されているかどうか(されているならtrue)
	bool isInited() const
	{
		return mIsInited;
	}

	//CPUからアクセス可能なリソースを取得する
	void* getResourceOnCPU(int bufferNo = 0);

	ID3D12Resource* getResource(int bufferNo = 0);

	void setDevice(ID3D12Device* dev);

private:
	RWStructuredBuffer(const RWStructuredBuffer&) = delete;
	RWStructuredBuffer& operator=(const RWStructuredBuffer&) = delete;

private:
	ID3D12Device* mDev;
	ID3D12Resource* mBufferOnGPU[2] = { nullptr };
	void* mBufferOnCPU[2] = { nullptr };
	int mNumElement;//要素数
	int mSizeOfElement;//エレメントのサイズ
	bool mIsInited;//初期化済みかどうか

};