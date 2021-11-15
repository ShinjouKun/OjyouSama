#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<map>
#include<string>


//�V�F�[�_�[�p
#include"d3dcompiler.h"

using namespace Microsoft::WRL;
using namespace std;
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

class PipeLine
{
public:
	PipeLine();
	~PipeLine();
	HRESULT SetPipeline3D(string shaderName);
	HRESULT SetPipeline2D(string shaderName);
	HRESULT SetPipelineParticle(string shaderName);
	HRESULT SetPipelineSequence(string shaderName);
	ID3D12PipelineState* GetPipeLineState(string shaderName);
	ID3D12RootSignature* GetRootSignature(string shaderName);
    ID3D12DescriptorHeap* GetDescripterHeap()const;//3d�p
	void Clear();
private:
	const int TexSRVCount = 512;//�e�N�X�`���ő喇��FixMe
	//�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
	ComPtr<ID3DBlob>vsBlob;//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>gsBlob;//�W�I���g���p
	ComPtr<ID3DBlob>psBlob;//�s�N�Z���V�F�[�_�I�u�W�F�N�g

	ComPtr<ID3DBlob>errorBlob;//�G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob>rootSigBlob;
static map<string,ComPtr<ID3D12PipelineState>>PipeLineState;//�p�C�v���C���X�e�[�g
static map<string,ComPtr<ID3D12RootSignature>>RootSignature;//���[�g�V�O�l�X�`��
static ComPtr<ID3D12DescriptorHeap>DescHeap;//�f�X�N���v�^�q�[�v3D




};
