 #pragma once
#include <vector>
#include <list>
#include <map>
#include<memory>
#include<iterator>
#include"../Utility/SmartPtr.h"
#include "../Math/Vector3.h"

using namespace std;
//���`�����؋�ԊǗ��N���X
#define TREEMANAGER_MAXLEVEL 7
class CollisonTree;
class BaseCollider;
class WorldSpace;
class OctTreeManager
{
protected:
	unsigned int m_uiDim;
	WorldSpace **ppWsAry;//���`��ԃ|�C���^�z��
	unsigned int m_iPow[TREEMANAGER_MAXLEVEL + 1];//�ׂ��搔�l�z��
	Vector3 m_W;//�̈�̕�
	Vector3 m_RgnMin;//�̈�̍ŏ��l
	Vector3 m_RgnMax;//�̈�̍ő�l
	Vector3 m_Unit;//�ŏ��̈�̕ӂ̒���
	DWORD m_dwWorldNum;//��Ԃ̐�
	unsigned int m_uiLevel;//�ŉ����x��

public:
	//�R���X�g���N�^
	OctTreeManager();
	//�f�X�g���N�^
	virtual ~OctTreeManager();
	//���`�����؂̍\�z
	bool Init(unsigned int Level, const Vector3&Min, const Vector3&Max);
	
	//�I�u�W�F�N�g�̓o�^
	bool Regist(Vector3* Min, Vector3* Max, CollisonTree* spOFT);

	//�Փ˔��胊�X�g�̍쐬
	DWORD GetAllCollisonList(vector<BaseCollider*> &ColVect);

	// ��ԓ��ŏՓ˃��X�g���쐬
	bool GetCollisonList(DWORD Elem, vector<BaseCollider*> &ColVect, list<BaseCollider*> &ColStac);
	
	//��Ԃ̐���
	bool CreateNewWorld(DWORD Elem);
	//���W�����Ԕԍ����Z�o
	DWORD GetMortonNumber(Vector3* Min, Vector3* Max);
	DWORD BitSeparateFor3D(BYTE n);
	DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z);
	DWORD GetPointElem(const Vector3& p);
};