#pragma once
#include"d3d12.h"
#include"d3dx12.h"
#include"wrl.h"
#include<string>
#include"TexLoader.h"
//#include"PipeLine.h"
#include "../Device/PipeLine.h"
//数学ライブラリ
#include"DirectXMath.h"
//#include "Vector2.h"
//#include "Vector3.h"
//#include "Vector4.h"
//#include "Matrix4.h"
#include "../Math/Math.h"
//シェーダー用
#include"d3dcompiler.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
using namespace Microsoft::WRL;
using namespace std;
struct SpriteConstBuffData
{
	Vector4 color;
	Matrix4 mat;
};
struct SpriteDatas
{
	//GPUリソースの生成
	ComPtr<ID3D12Resource> vertBuff;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Matrix4 matWorld;//ワールド行列
	Vector4 color;
	TexData* texData;//テクスチャ情報（番号込み）
	Vector2 texSize;//テクスチャのサイズ
	Vector2 ancPoint;//アンカーポイント
	Vector4 texUV;//テクスチャの切り取り
};
struct SpriteVert
{
	Vector3 pos;
	Vector2 uv;
};
class TexRenderer
{
public:
	TexRenderer(PipeLine* pipe);
	~TexRenderer();
	void Init();
	void CreateVert();//ポリゴンの作成
	void SetBuffer();//バッファーのセット
	void SetUV(const string& name, float left, float top, float right, float bottom, bool inversionX, bool inversionY);
	void SetAncPoint(const string& name, const Vector2& point);
	
	void AddTexture(const string& key, const string& name);//使うテクスチャを追加する
	void Draw(const string& name, const Vector3& pos, float angle, const Vector2& size, const Vector4& color);
	void DrawNumber(int num, const Vector2& pos, const Vector2& size);
	void numberSetUV();

	
	
private:
	string texName;//テクスチャ名
	SpriteDatas data;
	map<string, SpriteDatas>spriteList;//テクスチャのリスト
	float texSize;//サイズ
	vector<SpriteVert> vertex;
	HRESULT result;
	Matrix4 matProjection;//射影変換行列FixMe
	SpriteConstBuffData* constMap;
	PipeLine* pipeLine = nullptr;//パイプラインクラスの実体FixMe
};
