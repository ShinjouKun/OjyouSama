#pragma once

#include "../../Math/Math.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <list>
#include <vector>
#include <string>

class PipeLine;

class Sequence
{
	struct ConstBuffdata
	{
		Vector4 Color;
		Matrix4 mat;
	};
	
private:
	Sequence();
public:
	~Sequence();

	//終了処理
	void finalize();
	//インスタンスを返す
	static Sequence& instance();
	void setDev(ID3D12Device* dev);
	
	//描画情報をセット
	void set(int num, const Vector2& pos, const Vector2& size);
	//描画情報をセット
	void set(float num, const Vector2& pos, const Vector2& size);
	//描画処理
	void drawNumber(ID3D12GraphicsCommandList* cmdList, PipeLine* pipeLine);

private:
	//描画用のバッファを作る
	void createResouceBuff();
	void createBuff(const Vector2& xy = Vector2(0.f, 0.f), const Vector2& x2y2 = Vector2(1.f, 1.f));
	void createTexBuff(std::string texName);
	//変換
	std::string intToString(int number);
	int stringToInt(std::string src);
	std::wstring stringToWString(std::string src);

private:
	Sequence(const Sequence&) = delete;
	Sequence& operator=(const Sequence&) = delete;
	Sequence(Sequence&&) = delete;
	Sequence& operator=(Sequence&&) = delete;

private:
	static Sequence* mInstance;
	ID3D12Device* mDev;

	Matrix4 mMatProjection;

	std::list<int> mNums;//数字順(後ろにどんどん追加していくのでlistのほうが良い？)
	ID3D12DescriptorHeap* mDescHeap;//デスクリプタヒープ
	ID3D12Resource* mTexResource;//テクスチャ用リソース
	//描画データ
	UINT mVertexSize;//頂点数
	std::vector<D3D12_VERTEX_BUFFER_VIEW> mVertBuffViews;//頂点バッファビュー
	std::vector<ID3D12Resource*> mVertBuffs;//頂点バッファ
	std::vector<ID3D12Resource*> mConstBuffs;//定数バッファ
};