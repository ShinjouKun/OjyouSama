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

	//UAV�ɓo�^
	void RegistUnorderAccessView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

	//����������Ă��邩�ǂ���(����Ă���Ȃ�true)
	bool isInited() const
	{
		return mIsInited;
	}

	//CPU����A�N�Z�X�\�ȃ��\�[�X���擾����
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
	int mNumElement;//�v�f��
	int mSizeOfElement;//�G�������g�̃T�C�Y
	bool mIsInited;//�������ς݂��ǂ���

};