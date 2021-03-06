#include "TexRenderer.h"
//#include"Window.h"
#include"TexLoader.h"
//#include"DirectXManager.h"
#include "../Device/Window.h"
#include "../Device/DirectXManager.h"

TexRenderer::TexRenderer(PipeLine* pipe)
	:pipeLine(pipe)
{
	Init();
}

TexRenderer::~TexRenderer()
{
}

void TexRenderer::Init()
{
	matProjection.m[3][0] = -1.0f;
	matProjection.m[3][1] = 1.0f;
	matProjection.m[0][0] = 2.0f / Window::Window_Width;
	matProjection.m[1][1] = -2.0f / Window::Window_Height;
}

void TexRenderer::CreateVert()
{
	vertex.clear();
	vertex.resize(4);
	float texSize = 100.0f;//初期サイズ
	vertex[0].pos.x = 0.0f;//左下
	vertex[0].pos.y = texSize;
	vertex[0].pos.z = 0.0f;
	vertex[0].uv.x = 0.0f;
	vertex[0].uv.y = 1.0f;

	vertex[1].pos.x = 0.0f;//左上
	vertex[1].pos.y = 0.0f;
	vertex[1].pos.z = 0.0f;
	vertex[1].uv.x = 0.0f;
	vertex[1].uv.y = 0.0f;

	vertex[2].pos.x = texSize;//右下
	vertex[2].pos.y = texSize;
	vertex[2].pos.z = 0.0f;
	vertex[2].uv.x = 1.0f;
	vertex[2].uv.y = 1.0f;

	vertex[3].pos.x = texSize;//右上
	vertex[3].pos.y = 0.0f;
	vertex[3].pos.z = 0.0f;
	vertex[3].uv.x = 1.0f;
	vertex[3].uv.y = 0.0f;
}

void TexRenderer::SetBuffer()
{
	//struct SpriteConstBuffData* constMap = nullptr;
	//頂点バッファ作成
	result = DirectXManager::GetInstance()->Dev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertex[0])*vertex.size()),//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data.vertBuff));
	SpriteVert* vertMap;
	//バッファへデータを送信
	result = data.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertex.data(), sizeof(vertex[0])*vertex.size());
	data.vertBuff->Unmap(0, nullptr);
	//頂点バッファビュー
	data.vbView.BufferLocation = data.vertBuff->GetGPUVirtualAddress();
	data.vbView.SizeInBytes = sizeof(vertex[0])*vertex.size();
	data.vbView.StrideInBytes = sizeof(vertex[0]);
	//定数バッファ
	result = DirectXManager::GetInstance()->Dev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteConstBuffData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data.constBuff));

	//data.color = Vector4(0, 0, 0, 1);

	result = data.constBuff->Map(0, nullptr, (void**)&constMap);//マッピング
	constMap->color = data.color;
	constMap->mat = matProjection;
	data.constBuff->Unmap(0, nullptr);
}

void TexRenderer::SetUV(const string& name, float left, float top, float right, float bottom, bool inversionX, bool inversionY)
{
	auto d = spriteList[name];
	float Width = d.texSize.x;
	float Height = d.texSize.y;
	float Left = (0.0f, -d.ancPoint.x)*Width;
	float Right = (1.0f - d.ancPoint.x)*Width;
	float Top = (0.0f - d.ancPoint.y)*Height;
	float Bottom = (1.0f - d.ancPoint.y)*Height;
	//反転
	if (inversionX)
	{
		Left = -Left;
		Right = -Right;
	}

	if (inversionY)
	{
		Top = -Top;
		Bottom = -Bottom;
	}

	vertex[0].pos = Vector3(Left, Bottom, 0.0f);
	vertex[1].pos = Vector3(Left, Top, 0.0f);
	vertex[2].pos = Vector3(Right, Bottom, 0.0f);
	vertex[3].pos = Vector3(Right, Top, 0.0f);

	d.texUV = Vector4(left, top, right, bottom);
	vertex[0].uv = Vector2(left, bottom);
	vertex[1].uv = Vector2(left, top);
	vertex[2].uv = Vector2(right, bottom);
	vertex[3].uv = Vector2(right, top);

	SpriteVert* vertMap;
	//バッファへデータを送信
	result = data.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertex.data(), sizeof(vertex[0])*vertex.size());
	data.vertBuff->Unmap(0, nullptr);
	spriteList.emplace(name, d);
}

void TexRenderer::SetAncPoint(const string& key, const Vector2& point)
{
	auto d = spriteList[key];
	spriteList.erase(key);
	d.ancPoint = point;
	spriteList.emplace(key, d);
}

void TexRenderer::SetSize(const string & name, const Vector2 & size)
{
	auto d = spriteList[name];
	spriteList.erase(name);
	d.texSize = size;
	vertex[0].pos = Vector3(0.0f, d.texSize.y, 0.0f);//左下
	vertex[1].pos = Vector3(0.0f, 0.0f, 0.0f);//左上
	vertex[2].pos = Vector3(d.texSize.x, d.texSize.y, 0.0f);//右下
	vertex[3].pos = Vector3(d.texSize.x, 0.0f, 0.0f);//右上

	SpriteVert* vertMap;
	//バッファへデータを送信
	result = d.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertex.data(), sizeof(vertex[0])*vertex.size());
	d.vertBuff->Unmap(0, nullptr);

	spriteList.emplace(name, d);
}


void TexRenderer::AddTexture(const string& key, const string& filename)
{
	texName = key;//キーワード登録
	//TexNameをキーにテクスチャデータを呼び出してdataに登録する
	data.texData = &TexLoader::GetInstance(pipeLine)->GetTexList(filename);
	//頂点データを画像サイズに書き換え
	data.texSize.x = (float)data.texData->texResource->GetDesc().Width;
	data.texSize.y = (float)data.texData->texResource->GetDesc().Height;

	CreateVert();
	vertex[0].pos = Vector3(0.0f, data.texSize.y, 0.0f);//左下
	vertex[1].pos = Vector3(0.0f, 0.0f, 0.0f);//左上
	vertex[2].pos = Vector3(data.texSize.x, data.texSize.y, 0.0f);//右下
	vertex[3].pos = Vector3(data.texSize.x, 0.0f, 0.0f);//右上
	data.ancPoint = Vector2(0.0f, 0.0f);
	data.texUV = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	SetBuffer();
	spriteList.emplace(texName, data);//転送
}

void TexRenderer::Draw(const string& name, const Vector3 & pos, float angle, const Vector2 & size, const Vector4 & color)
{

	SpriteConstBuffData* constMap = nullptr;
	auto d = spriteList[name];
	d.color = color;
	d.texSize = size;

	d.matWorld = Matrix4::Identity;
	d.matWorld *= Matrix4::createScale(Vector3(d.texSize.x, d.texSize.y, 1.f));
	d.matWorld = Matrix4::createTranslation(Vector3(d.ancPoint.x, d.ancPoint.y, 0.0f));
	d.matWorld *= Matrix4::RotateZ(angle);
	d.matWorld *= Matrix4::createTranslation(pos);
	//行列の転送
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = d.matWorld*matProjection;
	//constMap->color = Vector4(0, 0, 0, 1);
	constMap->color = d.color;
	d.constBuff->Unmap(0, nullptr);
	//頂点バッファセット
	DirectXManager::GetInstance()->CmdList()->IASetVertexBuffers(0, 1, &d.vbView);
	//定数バッファのセット
	DirectXManager::GetInstance()->CmdList()->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//シェーダリソースビューセット
	DirectXManager::GetInstance()->CmdList()->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(pipeLine->GetDescripterHeap()->GetGPUDescriptorHandleForHeapStart(),
			d.texData->TexNum,//dataのTexDataのTexNUMを渡す
			DirectXManager::GetInstance()->Dev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//描画コマンド
	DirectXManager::GetInstance()->CmdList()->DrawInstanced(vertex.size(), 1, 0, 0);
}

void TexRenderer::DrawNumber(int num, const Vector2 & pos, const Vector2 & size)
{
	if (num < 0)
	{
		num = 0;
	}

	auto p = pos;
	auto str = std::to_string(num);

	for (auto n : str)
	{
		std::string s{ n };
		auto iNum = std::stoi(s);
		std::string name = "Number";
		name += s;

		auto d = spriteList[name];

		SpriteConstBuffData* constMap = nullptr;

		
		d.color = Vector4::one;
		d.texSize = size;

		d.matWorld = Matrix4::Identity;
		d.matWorld *= Matrix4::createScale(Vector3(size.x, size.y, 1.f));
		d.matWorld *= Matrix4::createTranslation(Vector3(p.x, p.y, 0.f));
		//行列の転送
		result = d.constBuff->Map(0, nullptr, (void**)&constMap);
		constMap->mat = d.matWorld*matProjection;
		//constMap->color = Vector4(0, 0, 0, 1);
		constMap->color = d.color;
		d.constBuff->Unmap(0, nullptr);
		//頂点バッファセット
		DirectXManager::GetInstance()->CmdList()->IASetVertexBuffers(0, 1, &d.vbView);
		//定数バッファのセット
		DirectXManager::GetInstance()->CmdList()->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
		//シェーダリソースビューセット
		DirectXManager::GetInstance()->CmdList()->SetGraphicsRootDescriptorTable(
			1,
			CD3DX12_GPU_DESCRIPTOR_HANDLE(pipeLine->GetDescripterHeap()->GetGPUDescriptorHandleForHeapStart(),
				d.texData->TexNum,//dataのTexDataのTexNUMを渡す
				DirectXManager::GetInstance()->Dev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

		//描画コマンド
		DirectXManager::GetInstance()->CmdList()->DrawInstanced(vertex.size(), 1, 0, 0);

		p.x += size.x;
	}

}

void TexRenderer::numberSetUV()
{
	
	for (int i = 0; i < 10; ++i)
	{
		std::string str = "Number";
		str += std::to_string(i);

		AddTexture(str, "Resouse/Number.png");
		auto d = spriteList[str];

		vertex[0].pos = Vector3(0.f, 1.f, 0.0f);
		vertex[1].pos = Vector3(0.f, 0.f, 0.0f);
		vertex[2].pos = Vector3(1.f, 1.f, 0.0f);
		vertex[3].pos = Vector3(1.f, 0.f, 0.0f);

		auto width = i / 10.f;
		d.texUV = Vector4(width, 0.f, width + 0.1f, 1.f);
		vertex[0].uv = Vector2(width, 1.f);
		vertex[1].uv = Vector2(width, 0.f);
		vertex[2].uv = Vector2(width + 0.1f, 1.f);
		vertex[3].uv = Vector2(width + 0.1f, 0.f);

		SpriteVert* vertMap;
		//バッファへデータを送信
		result = data.vertBuff->Map(0, nullptr, (void**)&vertMap);
		memcpy(vertMap, vertex.data(), sizeof(vertex[0])*vertex.size());
		data.vertBuff->Unmap(0, nullptr);
		spriteList.emplace(str, d);
	}
}

