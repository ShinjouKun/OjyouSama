#include "TexLoader.h"
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
//#include"DirectXManager.h"
#include "../Device/DirectXManager.h"
//DirectXTex
#include"DirectXTex.h"
using namespace DirectX;


namespace {
	const int dataSize = 24;
}
TexLoader::TexLoader(PipeLine* pipeline) :pipeLine(pipeline)
{

}

TexLoader::~TexLoader()
{
	texList.clear();
}

void TexLoader::Load(string filename)
{
	//���łɓo�^�ς݂Ȃ�X���[
	if (texList.find(filename) != texList.end())
	{
		return;
	}
	//WIC�e�N�X�`���̃��[�h
	wstring wf = wstring(filename.begin(), filename.end());
	const wchar_t *t = wf.c_str();
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	auto result = LoadFromWICFile(
		t,
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);
	const Image* img = scratchImg.GetImage(0, 0, 0);//���f�[�^���o
	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);


	TexData d;//�e�N�X�`���f�[�^
	//�e�N�X�`���p�o�b�t�@�̍쐬
	result = DirectXManager::GetInstance()->Dev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���w��p
		nullptr,
		IID_PPV_ARGS(&d.texResource)
	);
	//�e�N�X�`���o�b�t�@�Ƀf�[�^��]��
	result = d.texResource->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);


	//�V�F�[�_�[���\�[�X�r���[�쐬
	UINT descriptorHandleIncrementSize;

	descriptorHandleIncrementSize =
		DirectXManager::GetInstance()->Dev()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = d.texResource->GetDesc();//�󂯎��
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;


	DirectXManager::GetInstance()->Dev()->CreateShaderResourceView(d.texResource.Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(pipeLine->GetDescripterHeap()->GetCPUDescriptorHandleForHeapStart(),
			texList.size(),
			descriptorHandleIncrementSize));
	d.TexNum = texList.size();//�ԍ����킹
	//�ǉ�
	texList.emplace(filename, d);

}

TexData& TexLoader::GetTexList(const string& name)
{
	return texList[name];
}

TexLoader * TexLoader::GetInstance(PipeLine* pipeline)
{
	static TexLoader instance(pipeline);
	return &instance;
}

