#pragma once
#include "../../Math/Math.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <vector>

class RWStructuredBuffer;
class StructuredBuffer;
class Shader;

class Compute
{
	struct ParticleVertex
	{
		Vector3 pos;
		Vector4 color;
		Vector3 size;
		Vector3 rotate;
	};
	struct ParticleConstBuff
	{
		Matrix4 mat;
		Matrix4 matbillboard;
	};
public:
	Compute();
	~Compute();

	//�I������
	void finalize();

	void init();

	//�G�~�b�^�[�̍X�V
	void emitterUpdate(void* data, int dispatch = 1);
	//�p�[�e�B�N���̍X�V
	void* particleUpdate(void* data, int dataSize);
	//�p�[�e�B�N���̕`��
	void particleDraw(int dataSize);

private:
	void barrier(ID3D12Resource* p, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
	HRESULT createCMDList();
	HRESULT createHeap();
	HRESULT createPiprLine();
	HRESULT createBuffer();

private:
	Compute(const Compute&) = delete;
	Compute& operator=(const Compute&) = delete;

private:
	Shader* mCSShader;
	Shader* mPSShader;
	Shader* mGSShader;
	Shader* mVSShader;
	Shader* mParticleCSShader;

	StructuredBuffer* mInputEmitterSB;	   //�G�~�b�^�[�������ݐ�p
	RWStructuredBuffer* mOutputEmitterSB;  //�G�~�b�^�[�ǂݍ��ݐ�p
	StructuredBuffer* mInputParticleDataSB;//�p�[�e�B�N���������ݐ�p
	RWStructuredBuffer* mOutputParticleSB; //�p�[�e�B�N���ǂݍ��ݐ�p(�`��p)
	RWStructuredBuffer* mParticleStorageSB;//�p�[�e�B�N���ǂݍ��ݐ�p
	
	ID3D12RootSignature* mEmitterRoot;
	ID3D12PipelineState* mEmitterPipe;
	ID3D12RootSignature* mParticleRoot;
	ID3D12PipelineState* mParticlePipe;
	ID3D12RootSignature* mParticleDrawRoot;
	ID3D12PipelineState* mParticleDrawPipe;

	ID3D12DescriptorHeap* mHeap;

	ID3D12Device* mDev;
	ID3D12GraphicsCommandList* mCMDList;
	ID3D12CommandAllocator* mCMDAllo;
	ID3D12CommandQueue* mCQueue;

	//�`��p
	ID3D12Resource* mVertBuff;
	ID3D12Resource* mConstBuff;
	D3D12_VERTEX_BUFFER_VIEW mVbView{};
	ID3D12Resource* mTexBuff;

	Matrix4 mMatProjection3D;
};