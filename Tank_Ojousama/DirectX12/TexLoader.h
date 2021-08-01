#pragma once
#include"d3dx12.h"
#include"d3d12.h"
#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>
#include"wrl.h"
#include<map>
#include"PipeLine.h"
using namespace std;
using namespace Microsoft::WRL;
struct TexData
{
	ComPtr<ID3D12Resource>texResource;//�e�N�X�`���f�[�^
	UINT TexNum;
};
class TexLoader
{
public:
	TexLoader(PipeLine* pipeline);
	~TexLoader();
	void Load(string filename);
	//Get
	TexData& GetTexList(const string& name);

	//�C���X�^���X�쐬�@���[�h�N���X�̓V���O���g����
	static TexLoader* GetInstance(PipeLine* pipeline);

private:
	std::map<string, TexData>texList;//�摜�̃f�[�^���X�g
	PipeLine* pipeLine = nullptr;//�p�C�v���C���N���X�̎���FixMe
};
