#pragma once
#include"d3d12.h"
#include"d3dx12.h"
#include"wrl.h"
#include<string>
#include"TexLoader.h"
//#include"PipeLine.h"
#include "../Device/PipeLine.h"
//���w���C�u����
#include"DirectXMath.h"
//#include "Vector2.h"
//#include "Vector3.h"
//#include "Vector4.h"
//#include "Matrix4.h"
#include "../Math/Math.h"
//�V�F�[�_�[�p
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
	//GPU���\�[�X�̐���
	ComPtr<ID3D12Resource> vertBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Matrix4 matWorld;//���[���h�s��
	Vector4 color;
	TexData* texData;//�e�N�X�`�����i�ԍ����݁j
	Vector2 texSize;//�e�N�X�`���̃T�C�Y
	Vector2 ancPoint;//�A���J�[�|�C���g
	Vector4 texUV;//�e�N�X�`���̐؂���
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
	void CreateVert();//�|���S���̍쐬
	void SetBuffer();//�o�b�t�@�[�̃Z�b�g
	void SetUV(const string& name, float left, float top, float right, float bottom, bool inversionX, bool inversionY);
	void SetAncPoint(const string& name, const Vector2& point);
	
	void AddTexture(const string& key, const string& name);//�g���e�N�X�`����ǉ�����
	void Draw(const string& name, const Vector3& pos, float angle, const Vector2& size, const Vector4& color);
	void DrawNumber(int num, const Vector2& pos, const Vector2& size);
	void numberSetUV();

	
	
private:
	string texName;//�e�N�X�`����
	SpriteDatas data;
	map<string, SpriteDatas>spriteList;//�e�N�X�`���̃��X�g
	float texSize;//�T�C�Y
	vector<SpriteVert> vertex;
	HRESULT result;
	Matrix4 matProjection;//�ˉe�ϊ��s��FixMe
	SpriteConstBuffData* constMap;
	PipeLine* pipeLine = nullptr;//�p�C�v���C���N���X�̎���FixMe
};
