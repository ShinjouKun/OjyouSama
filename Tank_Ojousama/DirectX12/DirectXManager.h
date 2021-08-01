#pragma once
#include"wrl.h"
#include"d3d12.h"
#include"d3dx12.h"
#include"dxgi1_6.h"
#include"vector"
#include<memory>
#include<array>
//シェーダー用
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
#include"Window.h"
//FPS用
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

	void SetData3D();//モデルと3Dオブジェクト用
	void SetData2D();//スプライト用
	void SetDataParticle();//パーティクル用

	void SetDrawComnd();
	void PostEffctBegin();
	void PostEffctEnd();
	void PostEffctDraw();
	static DirectXManager* GetInstance();
	ID3D12Device* Dev()const;
	ID3D12GraphicsCommandList*CmdList()const;

	//FPS管理用
	long long cnt = 0;
	 const int limit = 60;//FPS値
	 int time = currentTime();
	 //現在の時間を獲得する関数
	 long long currentTime();
	 void UpdateFPS();
	 void Update();//全更新
private:
	void PostEffctBuff();
	
	//初期化用
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
	
	
	//バックバッファの番号獲得
	UINT bbIndex; 
	//デプスステンシルビュー
	ComPtr<ID3D12DescriptorHeap>dsvHeap;
	ComPtr<ID3D12Resource> depthBuffer;

	ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	//パイプラインクラス
    shared_ptr<PipeLine> pipeLine;
	//float r, g, b, a;//色
	//画面クリア時の色をここで設定 なるべく変更しないほうがいい（効率が落ちて警告が出る）
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;

//定数バッファ初期化
	struct ConstBufferData
	{
		XMFLOAT4 color;//色（RGBA）
		XMMATRIX mat;//3D変換行列
	};

	ConstBufferData* constMap = nullptr;
	ComPtr<ID3D12Resource> constBuff;

	//imgui関連
	ComPtr<ID3D12DescriptorHeap> imguiHeap;//ヒープ保持用
	ComPtr<ID3D12DescriptorHeap>Createimgui();//imgui作成
	ComPtr<ID3D12DescriptorHeap>Getimgui();//imguiGet
	//ポストエフェクト用
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
