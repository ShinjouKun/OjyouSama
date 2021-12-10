#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer() :
	mNumElement(0),
	mSizeOfElement(0),
	mIsInited(false)
{
}

StructuredBuffer::~StructuredBuffer()
{
	CD3DX12_RANGE readRange(0, 0);
	for (auto& buffer : mBufferOnGPU)
	{
		if (buffer)
		{
			buffer->Unmap(0, &readRange);
			buffer->Release();
			buffer = nullptr;
		}
	}
}

void StructuredBuffer::init(int sizeOfElement, int numElement, void * initData)
{
	mSizeOfElement = sizeOfElement;
	mNumElement = numElement;


	int bufferNo = 0;
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto rDesc = CD3DX12_RESOURCE_DESC::Buffer(mSizeOfElement * mNumElement);
	for (auto& buffer : mBufferOnGPU)
	{
		auto hr = mDev->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&rDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&buffer)
		);

		//構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
		//マップ、アンマップのオーバーヘッドを軽減するためにはこのインスタンスが生きている間は行わない。
		{
			CD3DX12_RANGE readRange(0, 0);
			buffer->Map(0, &readRange, reinterpret_cast<void**>(&mBufferOnCPU[bufferNo]));
		}
		if (initData != nullptr)
		{
			memcpy(mBufferOnCPU[bufferNo], initData, mSizeOfElement * mNumElement);
		}
		++bufferNo;

	}
	mIsInited = true;
}

void StructuredBuffer::RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
	//初期化されてなければリターン
	if (!mIsInited)return;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = static_cast<UINT>(mNumElement);
	srvDesc.Buffer.StructureByteStride = mSizeOfElement;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	mDev->CreateShaderResourceView(
		mBufferOnGPU[bufferNo],
		&srvDesc,
		descriptorHandle
	);

}

void StructuredBuffer::update(void * data, int dataSize)
{
	if (dataSize > mNumElement) dataSize = mNumElement;
	auto backBufferIndex = 0;
	memcpy(mBufferOnCPU[backBufferIndex], data, mSizeOfElement * dataSize);
}

ID3D12Resource * StructuredBuffer::getResource(int bufferNo)
{
	return mBufferOnGPU[bufferNo];
}

void StructuredBuffer::setDevice(ID3D12Device * dev)
{
	mDev = dev;
}
