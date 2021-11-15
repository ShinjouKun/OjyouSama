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

	//�I������
	void finalize();
	//�C���X�^���X��Ԃ�
	static Sequence& instance();
	void setDev(ID3D12Device* dev);
	
	//�`������Z�b�g
	void set(int num, const Vector2& pos, const Vector2& size);
	//�`������Z�b�g
	void set(float num, const Vector2& pos, const Vector2& size);
	//�`�揈��
	void drawNumber(ID3D12GraphicsCommandList* cmdList, PipeLine* pipeLine);

private:
	//�`��p�̃o�b�t�@�����
	void createResouceBuff();
	void createBuff(const Vector2& xy = Vector2(0.f, 0.f), const Vector2& x2y2 = Vector2(1.f, 1.f));
	void createTexBuff(std::string texName);
	//�ϊ�
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

	std::list<int> mNums;//������(���ɂǂ�ǂ�ǉ����Ă����̂�list�̂ق����ǂ��H)
	ID3D12DescriptorHeap* mDescHeap;//�f�X�N���v�^�q�[�v
	ID3D12Resource* mTexResource;//�e�N�X�`���p���\�[�X
	//�`��f�[�^
	UINT mVertexSize;//���_��
	std::vector<D3D12_VERTEX_BUFFER_VIEW> mVertBuffViews;//���_�o�b�t�@�r���[
	std::vector<ID3D12Resource*> mVertBuffs;//���_�o�b�t�@
	std::vector<ID3D12Resource*> mConstBuffs;//�萔�o�b�t�@
};