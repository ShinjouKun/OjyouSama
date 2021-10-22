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
	texSize = 100.0f;//�����T�C�Y
	vertex[0].pos.x = 0.0f;//����
	vertex[0].pos.y = texSize;
	vertex[0].pos.z = 0.0f;
	vertex[0].uv.x = 0.0f;
	vertex[0].uv.y = 1.0f;

	vertex[1].pos.x = 0.0f;//����
	vertex[1].pos.y = 0.0f;
	vertex[1].pos.z = 0.0f;
	vertex[1].uv.x = 0.0f;
	vertex[1].uv.y = 0.0f;

	vertex[2].pos.x = texSize;//�E��
	vertex[2].pos.y = texSize;
	vertex[2].pos.z = 0.0f;
	vertex[2].uv.x = 1.0f;
	vertex[2].uv.y = 1.0f;

	vertex[3].pos.x = texSize;//�E��
	vertex[3].pos.y = 0.0f;
	vertex[3].pos.z = 0.0f;
	vertex[3].uv.x = 1.0f;
	vertex[3].uv.y = 0.0f;
}

void TexRenderer::SetBuffer()
{
	//struct SpriteConstBuffData* constMap = nullptr;
	//���_�o�b�t�@�쐬
	result = DirectXManager::GetInstance()->Dev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertex[0])*vertex.size()),//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data.vertBuff));
	SpriteVert* vertMap;
	//�o�b�t�@�փf�[�^�𑗐M
	result = data.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertex.data(), sizeof(vertex[0])*vertex.size());
	data.vertBuff->Unmap(0, nullptr);
	//���_�o�b�t�@�r���[
	data.vbView.BufferLocation = data.vertBuff->GetGPUVirtualAddress();
	data.vbView.SizeInBytes = sizeof(vertex[0])*vertex.size();
	data.vbView.StrideInBytes = sizeof(vertex[0]);
	//�萔�o�b�t�@
	result = DirectXManager::GetInstance()->Dev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteConstBuffData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data.constBuff));

	//data.color = Vector4(0, 0, 0, 1);

	result = data.constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
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
	//���]
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
	//�o�b�t�@�փf�[�^�𑗐M
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


void TexRenderer::AddTexture(const string& key, const string& filename)
{
	texName = key;//�L�[���[�h�o�^
	//TexName���L�[�Ƀe�N�X�`���f�[�^���Ăяo����data�ɓo�^����
	data.texData = &TexLoader::GetInstance(pipeLine)->GetTexList(filename);
	//���_�f�[�^���摜�T�C�Y�ɏ�������
	data.texSize.x = (float)data.texData->texResource->GetDesc().Width;
	data.texSize.y = (float)data.texData->texResource->GetDesc().Height;
	CreateVert();
	vertex[0].pos = Vector3(0.0f, data.texSize.y, 0.0f);//����
	vertex[1].pos = Vector3(0.0f, 0.0f, 0.0f);//����
	vertex[2].pos = Vector3(data.texSize.x, data.texSize.y, 0.0f);//�E��
	vertex[3].pos = Vector3(data.texSize.x, 0.0f, 0.0f);//�E��
	data.ancPoint = Vector2(0.0f, 0.0f);
	data.texUV = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	SetBuffer();
	spriteList.emplace(texName, data);//�]��
}

void TexRenderer::Draw(const string& name, const Vector3 & pos, float angle, const Vector2 & size, const Vector4 & color)
{

	SpriteConstBuffData* constMap = nullptr;
	auto d = spriteList[name];
	d.color = color;
	d.texSize = size;

	d.matWorld = Matrix4::Identity;
	d.matWorld = Matrix4::createTranslation(Vector3(d.ancPoint.x, d.ancPoint.y, 0.0f));
	d.matWorld *= Matrix4::RotateZ(angle);
	d.matWorld *= Matrix4::createTranslation(pos);
	//�s��̓]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = d.matWorld*matProjection;
	//constMap->color = Vector4(0, 0, 0, 1);
	constMap->color = d.color;
	d.constBuff->Unmap(0, nullptr);
	//���_�o�b�t�@�Z�b�g
	DirectXManager::GetInstance()->CmdList()->IASetVertexBuffers(0, 1, &d.vbView);
	//�萔�o�b�t�@�̃Z�b�g
	DirectXManager::GetInstance()->CmdList()->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	DirectXManager::GetInstance()->CmdList()->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(pipeLine->GetDescripterHeap()->GetGPUDescriptorHandleForHeapStart(),
			d.texData->TexNum,//data��TexData��TexNUM��n��
			DirectXManager::GetInstance()->Dev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�`��R�}���h
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

		const constexpr float constant = 1.0f / 10.0f;

		float width = constant * iNum;

		std::string name = "";
		SetUV(name, width, 0.f, width + constant, 1.f, false, false);
		SpriteConstBuffData* constMap = nullptr;
		auto d = spriteList[name];
		d.color = Vector4::one;
		d.texSize = size;

		d.matWorld = Matrix4::Identity;
		d.matWorld *= Matrix4::createScale(Vector3(size.x, size.y, 0.f));
		d.matWorld *= Matrix4::createTranslation(Vector3(pos.x, pos.y, 0.f));
		//�s��̓]��
		result = d.constBuff->Map(0, nullptr, (void**)&constMap);
		constMap->mat = d.matWorld*matProjection;
		//constMap->color = Vector4(0, 0, 0, 1);
		constMap->color = d.color;
		d.constBuff->Unmap(0, nullptr);
		//���_�o�b�t�@�Z�b�g
		DirectXManager::GetInstance()->CmdList()->IASetVertexBuffers(0, 1, &d.vbView);
		//�萔�o�b�t�@�̃Z�b�g
		DirectXManager::GetInstance()->CmdList()->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
		//�V�F�[�_���\�[�X�r���[�Z�b�g
		DirectXManager::GetInstance()->CmdList()->SetGraphicsRootDescriptorTable(
			1,
			CD3DX12_GPU_DESCRIPTOR_HANDLE(pipeLine->GetDescripterHeap()->GetGPUDescriptorHandleForHeapStart(),
				d.texData->TexNum,//data��TexData��TexNUM��n��
				DirectXManager::GetInstance()->Dev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

		//�`��R�}���h
		DirectXManager::GetInstance()->CmdList()->DrawInstanced(vertex.size(), 1, 0, 0);

		p.x += size.x;
	}

}

