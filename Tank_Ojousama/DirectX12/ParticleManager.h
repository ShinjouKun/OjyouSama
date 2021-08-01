#pragma once

#pragma once
#include"d3d12.h"
#include"d3dx12.h"
#include"dxgi1_6.h"
#include<memory>
#include<iostream>
#include<unordered_map>
#include"wrl.h"
#include"vector"
#include<map>
#include<forward_list>
//�V�F�[�_�[�p
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
//�E�B���h�E
#include"Window.h"

//���w���C�u����
#include"DirectXMath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

//���[�_�[�ƃ����_���[
#include"TexLoader.h"
#include"PipeLine.h"
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
struct ConstBuff
{
	Matrix4 mat;
	Matrix4 matBill;
};
struct VertexParticle
{
	Vector3 pos;//�|�W�V����
	Vector4 color;
	float scale = 1.0f;//�T�C�Y
	Vector3 angle;
};
struct DirectVertexP
{
	XMFLOAT3 pos;
};
struct ParticleStateus//�p�[�e�B�N���ꗱ�̃X�e�[�^�X
{
	Vector3 pos;//�|�W�V����
	Vector3 vel;//���x
	Vector3 acc;//�����x
	Vector4 color;
	float scale = 1.0f;//�T�C�Y
	//�����X�P�[��
	float sScale = 1.0f;
	//�ŏI�X�P�[��
	float eScale = 0.0f;
	int frame = 0;//���݃t���[��
	int numFrame = 0;//�I���t���[��

	//�A���O��
	Vector3 angle;
};
struct ParticleData//�`��p
{
	std::vector<VertexParticle>vertex;
	//GPU���\�[�X�̐���
	ComPtr<ID3D12Resource> vertBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	TexData* texData;
	Vector3 ancPoint3D;
	//�p�[�e�B�N�����X�g
	std::forward_list<ParticleStateus>particles;
};

class ParticleManager
{
public:
	ParticleManager(PipeLine* pipe);
	~ParticleManager();
	void Init();
	void Update(const string& key);
	void OllUpDate();
	void CreateBuff(const string & key);
	void CreatePlane();//�p�[�e�B�N���p�̂����|��
	void CreateTexture(const string& filename);//�e�N�X�`���̓K�p(����Ȃ�����)
	void CreateParticleData(const string& key, const string& filename);//�p�[�e�B�N���������
	void DrawParticleBill(const string& key);//�S�����r���{�[�h
	void DrawParticleBillY(const string& key);//Y���r���{�[�h
	void OllDraw();
	//�p�[�e�B�N���p���X�g�ɒǉ�
	void Add(const string& key, int life, const Vector3& pos, const Vector3& vel, const Vector3& acc, float sScale, float eScale, const Vector4& color, const Vector3& angle);
private:
	PipeLine* pipeLine = nullptr;//�p�C�v���C���N���X�̎���FixMe
	//ConstBuff*constMap;//�萔�o�b�t�@�Ƀf�[�^��]��
	Matrix4 matProjectionP;//�ˉe�ϊ��s��
	//ParticleData dataP;
	HRESULT result;
	map<string, ParticleData>particleDatas;//�e�N�X�`���̃��X�g
	UINT texNum;//�e�N�X�`���ԍ�
	int vertCount = 1024;
};
class ParticleEmitterBox//�p�[�e�B�N���̔�����
{
public:
	ParticleEmitterBox(shared_ptr<ParticleManager> partM);
	~ParticleEmitterBox();
	void LoadAndSet(const string& key, const string& filename);//TexLoader���Ăяo���ATexRender�ɓn���ăo�b�t�@���Z�b�g����
	void EmitterUpdate(const string& key, const Vector3& pos, const Vector3& angle);//�l���ɍL����
	void EmitterUpdateBIG(const string& key, const Vector3& pos, const Vector3& angle);//�l���ɍL����
	void EmitterUpdateUpGas(const string& key, const Vector3& pos, const Vector3& angle);//��ɔr�o�����
	void EmitterUpdateFireGas(const string& key, const Vector3& pos, const Vector3& angle);//�r�[��
	void SetPos(Vector3 pos);
	void SetAngle(Vector3 angle);
private:
	PipeLine* pipeLine = nullptr;//�p�C�v���C���N���X�̎���FixMe
	Vector3 pos;
	Vector3 angle;
	shared_ptr<ParticleManager> particle;
};
