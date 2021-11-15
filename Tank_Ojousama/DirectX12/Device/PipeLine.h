#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<map>
#include<string>


//シェーダー用
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
    ID3D12DescriptorHeap* GetDescripterHeap()const;//3d用
	void Clear();
private:
	const int TexSRVCount = 512;//テクスチャ最大枚数FixMe
	//シェーダファイルの読み込みとコンパイル
	ComPtr<ID3DBlob>vsBlob;//頂点シェーダオブジェクト
	ComPtr<ID3DBlob>gsBlob;//ジオメトリ用
	ComPtr<ID3DBlob>psBlob;//ピクセルシェーダオブジェクト

	ComPtr<ID3DBlob>errorBlob;//エラーオブジェクト
	ComPtr<ID3DBlob>rootSigBlob;
static map<string,ComPtr<ID3D12PipelineState>>PipeLineState;//パイプラインステート
static map<string,ComPtr<ID3D12RootSignature>>RootSignature;//ルートシグネスチャ
static ComPtr<ID3D12DescriptorHeap>DescHeap;//デスクリプタヒープ3D




};
