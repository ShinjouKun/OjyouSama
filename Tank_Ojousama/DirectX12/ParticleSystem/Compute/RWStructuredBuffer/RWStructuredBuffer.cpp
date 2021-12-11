#include "RWStructuredBuffer.h"

RWStructuredBuffer::RWStructuredBuffer():
	mNumElement(0),
	mSizeOfElement(0),
	mIsInited(false)
{
}

RWStructuredBuffer::~RWStructuredBuffer()
{
	CD3DX12_RANGE readRenge(0, 0);
	for (auto& buffer : mBufferOnGPU)
	{
		if (buffer)
		{
			buffer->Unmap(0, &readRenge);
			buffer->Release();
			buffer = nullptr;
		}
	}
}

void RWStructuredBuffer::init(int sizeOfelement, int numElement, void * initData)
{
	mSizeOfElement = sizeOfelement;
	mNumElement = numElement;

	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(mSizeOfElement * mNumElement);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	int bufferNo = 0;

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask = 1;

	for (auto& buffer : mBufferOnGPU)
	{
		mDev->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&buffer)
		);
		//�\�����o�b�t�@��CPU����A�N�Z�X�\�ȉ��z�A�h���X��ԂɃ}�b�s���O����B
		//�}�b�v�A�A���}�b�v�̃I�[�o�[�w�b�h���y�����邽�߂ɂ͂��̃C���X�^���X�������Ă���Ԃ͍s��Ȃ��B
		{
			CD3DX12_RANGE readRange(0, 0);
			buffer->Map(0, &readRange, reinterpret_cast<void**>(&mBufferOnCPU[bufferNo]));
		}
		if (initData != nullptr)
		{
			memcpy(mBufferOnGPU[bufferNo], initData, mSizeOfElement * mNumElement);
		}
		++bufferNo;
	}
	mIsInited = true;
}

void RWStructuredBuffer::RegistUnorderAccessView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
	//����������ĂȂ���΃��^�[��
	if (!mIsInited)return;

	//auto dev = DX12Manager::instance().getDev()->getDev();
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements = static_cast<UINT>(mNumElement);
	desc.Buffer.StructureByteStride = mSizeOfElement;
	mDev->CreateUnorderedAccessView(
		mBufferOnGPU[bufferNo],
		nullptr,
		&desc,
		descriptorHandle
	);
}

void * RWStructuredBuffer::getResourceOnCPU(int bufferNo)
{
	return mBufferOnCPU[bufferNo];
}

ID3D12Resource * RWStructuredBuffer::getResource(int bufferNo)
{
	return mBufferOnGPU[bufferNo];
}

void RWStructuredBuffer::setDevice(ID3D12Device * dev)
{
	mDev = dev;
}
