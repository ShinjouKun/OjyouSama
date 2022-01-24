#include "Sequence.h"
#include "../WCHAR_char.h"
#include "../../Device/PipeLine.h"
#include "../../Device/Window.h"

#include <DirectXTex.h>
using namespace DirectX;

#include <cassert>

Sequence::Sequence():
	mConstBuffs(),
	mDev(nullptr),
	mDescHeap(nullptr),
	mTexResource(nullptr),
	mVertexSize(4),
	mVertBuffViews(),
	mVertBuffs(),
	mNums(),
	mMatProjection(Matrix4::Identity)
{
	mNums.clear();
	mVertBuffViews.clear();
	mVertBuffs.clear();
	mConstBuffs.clear();

	

	mMatProjection = Matrix4::Identity;
	//原点をスクリーン左上にする
	mMatProjection.m[3][0] = -1.f;
	mMatProjection.m[3][1] = 1.f;
	//ピクセル単位で扱うために
	mMatProjection.m[0][0] = 2.f / Window::Window_Width;
	mMatProjection.m[1][1] = -2.f / Window::Window_Height;

}

Sequence::~Sequence()
{
}

void Sequence::finalize()
{
	mNums.clear();
	mVertBuffViews.clear();
	mDescHeap->Release();
	mDescHeap = nullptr;

	mTexResource->Release();
	mTexResource = nullptr;

	for (auto& v : mVertBuffs)
	{
		v->Release(); 
		v = nullptr;
	}
	mVertBuffs.clear();

	for (auto& c : mConstBuffs)
	{
		c->Release();
		c = nullptr;
	}
	mConstBuffs.clear();
		
	delete(mInstance);
	mInstance = nullptr;
}

Sequence& Sequence::instance()
{
	if (!mInstance)
	{
		mInstance = new Sequence();
	}
	return *mInstance;
}

void Sequence::setDev(ID3D12Device * dev)
{
	if (mDev) return;
	mDev = dev;
	const constexpr float constant = 1.0f / 10.0f;//10等分にする(小数点ありなら11等分にする)

	for (int i = 0; i < 10; ++i)//0～9の10個作る
	{
		float width = constant * static_cast<float>(i);//uvの位置
		createBuff(Vector2(width, 0.f), Vector2(width + constant, 1.f));
	}
	//デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 512;
	mDev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&mDescHeap));

	createTexBuff("Resouse/Number1.png");
}

void Sequence::set(int num, const Vector2 & pos, const Vector2 & size)
{
	if (num < 0)
	{
		num = 0;
	}

	auto p = pos;
	auto str = intToString(num);//intからstringに変換
	for (auto& n : str)
	{
		createResouceBuff();
		std::string s{ n };//charからstringに変換
		auto number = stringToInt(s);//stringからintに変換


		Matrix4 mat = Matrix4::Identity;
		mat *= Matrix4::createScale(Vector3(size.x, size.y, 0.f));
		mat *= Matrix4::createTranslation(Vector3(p.x, p.y, 0.f));

		//map
		ConstBuffdata* constMap;
		mConstBuffs[mNums.size()]->Map(0, nullptr, (void**)&constMap);
		constMap->mat = mat * mMatProjection;
		constMap->Color = Vector4::one;
		mConstBuffs[mNums.size()]->Unmap(0, nullptr);
		//map

		mNums.emplace_back(number);

		p.x += size.x;
	}

}

void Sequence::set(float num, const Vector2 & pos, const Vector2 & size)
{
	set(static_cast<int>(num), pos, size);

	//必要なら追加する
}

void Sequence::drawNumber(ID3D12GraphicsCommandList* cmdList)
{
	//数字を描画しないのならリターン
	if (mNums.empty())return;

	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = mDescHeap->GetCPUDescriptorHandleForHeapStart();
	UINT64 descHandleIncrementSize = mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	D3D12_RESOURCE_DESC resDesc = mTexResource->GetDesc();

	for (int i = 0, end = static_cast<int>(mNums.size()); i < end; ++i)
	{
		//CBVを生成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.BufferLocation = mConstBuffs[i]->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = mConstBuffs[i]->GetDesc().Width;
		mDev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);
		//消費した分だけアドレスをずらす
		basicHeapHandle.ptr += descHandleIncrementSize;
	}

	//SRVを生成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	mDev->CreateShaderResourceView(mTexResource, &srvDesc, basicHeapHandle);
	//消費した分だけアドレスをずらす
	basicHeapHandle.ptr += descHandleIncrementSize;



	//デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeap[] = { mDescHeap };
	cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	//パイプライン設定
	cmdList->SetPipelineState(mPipeLine->GetPipeLineState("ahokusa"));
	//ルートシグネスチャ生成
	cmdList->SetGraphicsRootSignature(mPipeLine->GetRootSignature("ahokusa"));
	//プリミティブ形状
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//GPUのアドレス
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV = mDescHeap->GetGPUDescriptorHandleForHeapStart();

	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = gpuDescHandleCBV;
	gpuDescHandleSRV.ptr += descHandleIncrementSize * (static_cast<UINT64>(mNums.size()));

	for (auto& n : mNums)
	{
		//頂点バッファのセット
		cmdList->IASetVertexBuffers(0, 1, &mVertBuffViews[n]);

		//定数バッファのセット
		cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandleCBV);

		//シェーダリソースビューのセット
		cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);

		//描画コマンド
		cmdList->DrawInstanced(mVertexSize, 1, 0, 0);

		gpuDescHandleCBV.ptr += descHandleIncrementSize;
	}

	mNums.clear();
}

void Sequence::setPipeLine(PipeLine * pipeLine)
{
	mPipeLine = pipeLine;
}

void Sequence::createResouceBuff()
{
	if (mConstBuffs.size() > mNums.size()) return;
	//それぞれ位置が異なるため、描画分用意する
	ID3D12Resource* constBuff;
	HRESULT res;
	//定数バッファの生成
	res = mDev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffdata) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	assert(SUCCEEDED(res));

	mConstBuffs.emplace_back(constBuff);
}

void Sequence::createBuff(const Vector2 & xy, const Vector2 & x2y2)
{

	struct Vertex
	{
		Vector3 pos;
		Vector2 uv;
	};

	std::vector<Vertex> vertex;
	vertex.resize(4);
	vertex[0].pos.x = 0.0f;//左下
	vertex[0].pos.y = 1.0f;
	vertex[0].pos.z = 0.0f;
	vertex[0].uv.x = xy.x;
	vertex[0].uv.y = x2y2.y;

	vertex[1].pos.x = 0.0f;//左上
	vertex[1].pos.y = 0.0f;
	vertex[1].pos.z = 0.0f;
	vertex[1].uv.x = xy.x;
	vertex[1].uv.y = xy.y;

	vertex[2].pos.x = 1.0f;//右下
	vertex[2].pos.y = 1.0f;
	vertex[2].pos.z = 0.0f;
	vertex[2].uv.x = x2y2.x;
	vertex[2].uv.y = x2y2.y;

	vertex[3].pos.x = 1.0f;//右上
	vertex[3].pos.y = 0.0f;
	vertex[3].pos.z = 0.0f;
	vertex[3].uv.x = x2y2.x;
	vertex[3].uv.y = xy.y;

	ID3D12Resource* vertBuff;

	HRESULT result;
	// 頂点バッファ生成
	result = mDev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertex[0])*vertex.size()),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	assert(SUCCEEDED(result));

	//頂点バッファへデータ転送
	Vertex* vertMap;
	vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertex.data(), sizeof(vertex[0])*vertex.size());
	vertBuff->Unmap(0, nullptr);

	D3D12_VERTEX_BUFFER_VIEW view;
	view.BufferLocation = vertBuff->GetGPUVirtualAddress();
	view.SizeInBytes = static_cast<UINT>(sizeof(vertex[0])*vertex.size());
	view.StrideInBytes = static_cast<UINT>(sizeof(vertex[0]));

	mVertBuffs.emplace_back(vertBuff);
	mVertBuffViews.emplace_back(view);
}

void Sequence::createTexBuff(std::string texName)
{
	HRESULT hr;
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	
	hr = LoadFromWICFile(
		stringToWString(texName).c_str(),
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	assert(SUCCEEDED(hr));

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出
	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	// テクスチャ用バッファの生成
	hr = mDev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&mTexResource));

	assert(SUCCEEDED(hr));

	// テクスチャバッファにデータ転送
	hr = mTexResource->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		static_cast<UINT>(img->rowPitch),  // 1ラインサイズ
		static_cast<UINT>(img->slicePitch) // 1枚サイズ
	);

	assert(SUCCEEDED(hr));
}

std::string Sequence::intToString(int number)
{
	return std::to_string(number);
}

int Sequence::stringToInt(std::string src)
{
	auto num = std::stoi(src);
	return num;
}

std::wstring Sequence::stringToWString(std::string src)
{
	size_t newsize = strlen(src.c_str()) + 1;
	wchar_t* w = new wchar_t[newsize];
	size_t convertedChars = 0;

	mbstowcs_s(&convertedChars, w, newsize, src.c_str(), _TRUNCATE);

	std::wstring ws = w;
	delete(w);

	return ws;
}

Sequence* Sequence::mInstance = nullptr;
PipeLine* Sequence::mPipeLine = nullptr;