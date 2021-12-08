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

class StructuredBuffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();

	void init(int sizeOfElement, int numElement, void* initData);

	//SRVに登録
	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

	void update(void * data, int dataSize);

	bool isInited()const
	{
		return mIsInited;
	}

	ID3D12Resource* getResource(int bufferNo = 0);

	void setDevice(ID3D12Device* dev);

private:
	StructuredBuffer(const StructuredBuffer&) = delete;
	StructuredBuffer& operator=(const StructuredBuffer&) = delete;

private:
	ID3D12Device* mDev;
	ID3D12Resource* mBufferOnGPU[2] = { nullptr };
	void* mBufferOnCPU[2] = { nullptr };
	int mNumElement;//要素数。
	int mSizeOfElement;//エレメントのサイズ。
	bool mIsInited;//初期化してるかどうか
};