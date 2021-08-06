#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include <map>
#include <string>
#include<wrl.h>
#include<iostream>
#include<DirectXMath.h>
#include "ModelLoader.h"
//#include"Vector4.h"
//#include"Matrix4.h"
//#include"PipeLine.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix4.h"
#include "../Device/PipeLine.h"
#include"TexLoader.h"
using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
struct ConstMap
{
	Vector4 color;
	Matrix4 mat;
	XMFLOAT4 MatAmbinent;
	XMFLOAT4 MatDiffuse;
	XMFLOAT4 MatSpecular;
};
struct DrawData
{
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	Matrix4 matWorld;//���[���h�s
	TexData* texData;
	Vector3 ancPoint3D;
	//GPU���\�[�X�̐�
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource>vertBuff;
	ComPtr<ID3D12Resource>indexBuff;
	Vector4 color;
};
class ModelRenderer
{
private:
	map<string, OBJData*> datas;//�ۑ��p
	DrawData drawData;
	map<string, DrawData>drawDatas;
	PipeLine* pipeLine = nullptr;//�p�C�v���C���N���X�̎���FixMe
	string modelName;
	Matrix4 matProjection;//�ˉe�ϊ��s��


	//���u��
	void SetBuffer(OBJData* data);
public:
	ModelRenderer(PipeLine* pipeline);
	~ModelRenderer();
	void Init();
	void AddModel(const string& key, const string& filename, const string& texName);//�g�����f���̒ǉ�
	void SetMaterial(OBJMatM* matData, ConstMap* map);//�}�e���A���̃Z�b�g
	void Draw(const string& key, const Vector3& pos, const Vector3& angle, const Vector3& scale);

	//�A���J�[�|�C���g�ύX
	void SetAncPoint(const string& key, const Vector3& point);
	//�J���[�ύX
	void SetColor(const string& key, const Vector4& color);

};
