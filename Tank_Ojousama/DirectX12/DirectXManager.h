#pragma once
#include"wrl.h"
#include"d3d12.h"
#include"d3dx12.h"
#include"dxgi1_6.h"
#include"vector"
#include<memory>
#include<array>
//�V�F�[�_�[�p
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
#include"Window.h"
//FPS�p
#include<thread>
#include<chrono>
//imgui
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

class PipeLine;

class DirectXManager
{
public:
	DirectXManager();
	~DirectXManager();
	void Init(Window* window);
	void Begin();
	void End();

	void SetData3D();//���f����3D�I�u�W�F�N�g�p
	void SetData2D();//�X�v���C�g�p
	void SetDataParticle();//�p�[�e�B�N���p

	void SetDrawComnd();
	void PostEffctBegin();
	void PostEffctEnd();
	void PostEffctDraw();
	static DirectXManager* GetInstance();
	ID3D12Device* Dev()const;
	ID3D12GraphicsCommandList*CmdList()const;

	//FPS�Ǘ��p
	long long cnt = 0;
	 const int limit = 60;//FPS�l
	 int time = currentTime();
	 //���݂̎��Ԃ��l������֐�
	 long long currentTime();
	 void UpdateFPS();
	 void Update();//�S�X�V
private:
	void PostEffctBuff();
	
	//�������p
	HRESULT result;
	ComPtr<ID3D12Device>dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	
	
	//�o�b�N�o�b�t�@�̔ԍ��l��
	UINT bbIndex; 
	//�f�v�X�X�e���V���r���[
	ComPtr<ID3D12DescriptorHeap>dsvHeap;
	ComPtr<ID3D12Resource> depthBuffer;

	ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	//�p�C�v���C���N���X
    shared_ptr<PipeLine> pipeLine;
	//float r, g, b, a;//�F
	//��ʃN���A���̐F�������Őݒ� �Ȃ�ׂ��ύX���Ȃ��ق��������i�����������Čx�����o��j
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;

//�萔�o�b�t�@������
	struct ConstBufferData
	{
		XMFLOAT4 color;//�F�iRGBA�j
		XMMATRIX mat;//3D�ϊ��s��
	};

	ConstBufferData* constMap = nullptr;
	ComPtr<ID3D12Resource> constBuff;

	//imgui�֘A
	ComPtr<ID3D12DescriptorHeap> imguiHeap;//�q�[�v�ێ��p
	ComPtr<ID3D12DescriptorHeap>Createimgui();//imgui�쐬
	ComPtr<ID3D12DescriptorHeap>Getimgui();//imguiGet
	//�|�X�g�G�t�F�N�g�p
	ComPtr<ID3D12PipelineState>postEffctPipeLine;
	ComPtr<ID3D12RootSignature>postEffctRootSignature;
	ComPtr<ID3D12Resource>postEffctVB;
	D3D12_VERTEX_BUFFER_VIEW postEffctVBV;
	ComPtr<ID3D12DescriptorHeap>postEffctRTVheap;
	ComPtr<ID3D12DescriptorHeap>postEffctSRVheap;
	ComPtr<ID3D12Resource>postEffctCB;
	ComPtr<ID3D12DescriptorHeap>postEffctCBVheap;

	std::array<ComPtr<ID3D12Resource>, 2>postEffctResouce;

};
