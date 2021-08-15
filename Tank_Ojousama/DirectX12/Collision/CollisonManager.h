#pragma once
#include<forward_list>
#include<vector>
#include <vector>
#include <list>
#include <map>
#include<iterator>
//#include"BaseObject.h"
#include "../Actor/BaseObject.h"
#include"../Utility/SmartPtr.h"
using namespace std;
class BaseCollider;
template<class  T>
class WorldSpace;//�����蔻�蕪���p�̋�ԃN���X
template<class  T>
class CollisonTree//�����蔻�蕶�ؓo�^�N���X
{
public:
	WorldSpace<T> *wSpace;//�o�^���
	T *colObject;//����Ώ�
	sp<CollisonTree<T>>m_spPre;//�O��Tree�\����
	sp<CollisonTree<T>>m_spNext;//����Tree�\����
public:
	CollisonTree()
	{
		wSpace = NULL;
		colObject = NULL;
	}

	virtual ~CollisonTree(){}

public:
	//���X�g����̏��O
	bool Remove()
	{
		if (!wSpace)//���łɈ�E���Ă���ꍇ
			return false;
		if (!wSpace->OnRemove(this))//������o�^���Ă����ԂɎ��g��ʒm
			return false;
		//��E����
		//�O��̃I�u�W�F�N�g�����т���
		if (m_spPre.GetPtr() != NULL)
		{
			m_spPre->m_spNext = m_spNext;
			m_spPre.SetPtr(NULL);
		}
		if (m_spNext.GetPtr() != NULL)
		{
			m_spNext->m_spPre = m_spPre;
			m_spNext.SetPtr(NULL);
		}
		wSpace = NULL;
		return true;
	}
	//��Ԃ�o�^
	void RegistWorld(WorldSpace<T> *wSpa)
	{
		wSpace = wSpa;
	}
	//���̃I�u�W�F�N�g�ւ̃X�}�[�g�|�C���^���m��
	sp<CollisonTree<T>>&GetNextObj() { return m_spNext; }
};
//���`�����؋�ԊǗ��N���X
#define TREEMANAGER_MAXLEVEL 7
template<class T>
class OctTreeManager
{
protected:
	unsigned int m_uiDim;
	WorldSpace<T> **ppWsAry;//���`��ԃ|�C���^�z��
	unsigned int m_iPow[TREEMANAGER_MAXLEVEL + 1];//�ׂ��搔�l�z��
	Vector3 m_W;//�̈�̕�
	Vector3 m_RgnMin;//�̈�̍ŏ��l
	Vector3 m_RgnMax;//�̈�̍ő�l
	Vector3 m_Unit;//�ŏ��̈�̕ӂ̒���
	DWORD m_dwWorldNum;//��Ԃ̐�
	unsigned int m_uiLevel;//�ŉ����x��
public:
	//�R���X�g���N�^
	OctTreeManager()
	{
		m_uiDim = 0;
		ppWsAry = NULL;
		m_W.x = m_W.y = m_W.z = 1.0f;
		m_RgnMin.x = m_RgnMin.y = m_RgnMin.z = 0.0f;
		m_RgnMax.x = m_RgnMax.y = m_RgnMax.z = 1.0f;
		m_Unit.x = m_Unit.y = m_Unit.z = 1.0f;
		m_dwWorldNum = 0;
		m_uiLevel = 0;
	}
	//�f�X�g���N�^
	virtual ~OctTreeManager()
	{
		DWORD i;
		for (i = 0; i < m_dwWorldNum; i++)
		{
			if (ppWsAry[i] != NULL)
				delete ppWsAry[i];
		}
		delete[] ppWsAry;
	}
	//���`�����؂̍\�z
	bool Init(unsigned int Level, const Vector3&Min, const Vector3&Max)
	{
		// �ݒ�ō����x���ȏ�̋�Ԃ͍��Ȃ�
		if (Level >= TREEMANAGER_MAXLEVEL)
			return false;
		// �e���x���ł̋�Ԑ����Z�o
		int i;
		m_iPow[0] = 1;
		for (i = 1; i < TREEMANAGER_MAXLEVEL + 1; i++)
			m_iPow[i] = m_iPow[i - 1] * 8;
		// Level���x���i0��_�j�̔z��쐬
		m_dwWorldNum = (m_iPow[Level + 1] - 1) / 7;
		ppWsAry = new WorldSpace<T>*[m_dwWorldNum];
		ZeroMemory(ppWsAry, sizeof(WorldSpace<T>*)*m_dwWorldNum);

		//�̈�̓o�^
		m_RgnMin = Min;
		m_RgnMax = Max;
		m_W = m_RgnMax - m_RgnMin;
		m_Unit = m_W / ((float)(1 << Level));

		m_uiLevel = Level;

		return true;
	}
	//�I�u�W�F�N�g�̓o�^
	bool Regist(Vector3* Min, Vector3* Max, sp<CollisonTree<T>>&spOFT)
	{
		// �I�u�W�F�N�g�̋��E�͈͂���o�^���[�g���ԍ����Z�o
		DWORD Elem = GetMortonNumber(Min, Max);
		if (Elem < m_dwWorldNum)
		{
			//��Ԃ��Ȃ��ꍇ�͐V�������
			if (!ppWsAry[Elem])
				CreateNewWorld(Elem);
			return ppWsAry[Elem]->Push(spOFT);
		}
		return false;//�o�^���s
	}
	//�Փ˔��胊�X�g�̍쐬
	DWORD GetAllCollisonList(vector<T*> &ColVect)
	{
		//������
		ColVect.clear();
		//���[�g��Ԃ̑��݂��m�F
		if (ppWsAry[0] == NULL)
			return 0;//�����Ȃ�
		//���[�g��Ԃ̏���
		list<T*>ColStac;
		GetCollisonList(0, ColVect, ColStac);

		return (DWORD)ColVect.size();
	}
protected:
	// ��ԓ��ŏՓ˃��X�g���쐬
	bool GetCollisonList(DWORD Elem, vector<T*> &ColVect, list<T*> &ColStac)
	{
		typename list<T*>::iterator it;
		//��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
		sp<CollisonTree<T>> sp0FT1 = ppWsAry[Elem]->GetFirstObj();
		while (sp0FT1.GetPtr()!=NULL)
		{
			sp<CollisonTree<T>>sp0FT2 = sp0FT1->m_spNext;
			while (sp0FT2 !=NULL)
			{
				//�Փ˃��X�g�쐬
				ColVect.push_back(sp0FT1->colObject);
				ColVect.push_back(sp0FT2->colObject);
				sp0FT2 = sp0FT2->m_spNext;
			}
			//�Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
			for (it = ColStac.begin(); it != ColStac.end(); it++)
			{
				ColVect.push_back(sp0FT1->colObject);
				ColVect.push_back(*it);
			}
			sp0FT1 = sp0FT1->m_spNext;
		}
		bool ChildFlag = false;
		//�q��ԂɈړ�
		DWORD objNum = 0;
		DWORD i, NextElem;
		for (i = 0; i < 8; i++)
		{
			NextElem = Elem * 8 + 1 + i;
			if (NextElem < m_dwWorldNum && ppWsAry[Elem * 8 + 1 + i])
			{
				if (!ChildFlag)
				{
					//�o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
					sp0FT1 = ppWsAry[Elem]->GetFirstObj();
					while (sp0FT1.GetPtr())
					{
						ColStac.push_back(sp0FT1->colObject);
						objNum++;
						sp0FT1 = sp0FT1->m_spNext;
					}
				}
				ChildFlag = true;
				GetCollisonList(Elem * 8 + 1 + i, ColVect, ColStac);//�q��Ԃ�
			}
		}
		//�X�^�b�N����I�u�W�F�N�g���O��
		if (ChildFlag)
		{
			for (i=0;i<objNum;i++)
			{
				ColStac.pop_back();
			}
			return true;
		}

	}
	//��Ԃ̐���
	bool CreateNewWorld(DWORD Elem)
	{
		//�����̗v�f�ԍ�
		while (!ppWsAry[Elem])
		{
			ppWsAry[Elem] = new WorldSpace<T>;

			//�e��ԂɃW�����v
			Elem = (Elem - 1) >> 3;
			if (Elem >= m_dwWorldNum)break;
		}
		return true;
	}
	//���W�����Ԕԍ����Z�o
	DWORD GetMortonNumber(Vector3* Min, Vector3* Max)
	{
		// �ŏ����x���ɂ�����e���ʒu���Z�o
		DWORD LT = GetPointElem(*Min);
		DWORD RB = GetPointElem(*Max);
		// ��Ԕԍ��������Z���čŏ�ʋ�؂肩�珊�����x�����Z�o
		DWORD Def = RB ^ LT;
		unsigned int HiLevel = 1;
		unsigned int i;
		for (i = 0; i < m_uiLevel; i++)
		{
			DWORD Check = (Def >> (i * 3)) & 0x7;
			if (Check != 0)
				HiLevel = i + 1;
		}
		DWORD SpaceNum = RB >> (HiLevel * 3);
		DWORD AddNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 7;
		SpaceNum += AddNum;

		if (SpaceNum > m_dwWorldNum)
			return 0xffffffff;

		return SpaceNum;
	}
	// �r�b�g�����֐�
	DWORD BitSeparateFor3D(BYTE n)
	{
		DWORD s = n;
		s = (s | s << 8) & 0x0000f00f;
		s = (s | s << 4) & 0x000c30c3;
		s = (s | s << 2) & 0x00249249;
		return s;
	}

	// 3D���[�g����Ԕԍ��Z�o�֐�
	DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
	{
		return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
	}

	// ���W�����`8���ؗv�f�ԍ��ϊ��֐�
	DWORD GetPointElem(const Vector3& p)
	{
		return Get3DMortonNumber(
			(BYTE)((p.x - m_RgnMin.x) / m_Unit.x),
			(BYTE)((p.y - m_RgnMin.y) / m_Unit.y),
			(BYTE)((p.z - m_RgnMin.z) / m_Unit.z)
		);
	}
};
//��ԃN���X
template<class T>
class WorldSpace
{
protected:
	sp<CollisonTree<T>>m_spLatest;//�ŐVOFT�ւ̃X�}�[�g�|�C���^
public:
	WorldSpace() 
	{
	}
	virtual ~WorldSpace()
	{
		if (m_spLatest.GetPtr() != NULL)
		ResetLink(m_spLatest);
	}
	//�����N��S�ă��Z�b�g����
	void ResetLink(sp<CollisonTree<T>> &sp0FT)
	{
		if (sp0FT->m_spNext.GetPtr()!= NULL)
			ResetLink(sp0FT->m_spNext);
		sp0FT.SetPtr(NULL);//�X�}�[�g�|�C���^���Z�b�g
	}
	//0FT���v�b�V��
	bool Push(sp<CollisonTree<T>> &sp0FT)
	{
		if (sp0FT.GetPtr() == NULL)return false;//�����ȃI�u�W�F�N�g�͓o�^���Ȃ�
		if (sp0FT->wSpace == this)return false;//��d�o�^���`�F�b�N
		if (m_spLatest.GetPtr() == NULL)
		{
			m_spLatest = sp0FT;//��ԂɐV�K�o�^
		}
		else
		{
			//�ŐV0FT�I�u�W�F�N�g���X�V
			sp0FT->m_spNext = m_spLatest;
			m_spLatest->m_spPre = sp0FT;
			m_spLatest = sp0FT;
		}
		sp0FT->RegistWorld(this);//��Ԃ�o�^
		return true;
	}
	sp<CollisonTree<T>>&GetFirestObj()
	{
		return m_spLatest;
	}
	//�폜�����I�u�W�F�N�g�̃`�F�b�N
	bool OnRemove(CollisonTree<T> *pRemoveObj)
	{
		if (m_spLatest.GetPtr() == pRemoveObj)
		{
			//���̃I�u�W�F�N�g�ɑ}���ւ�
			if (m_spLatest.GetPtr() != NULL)
				m_spLatest = m_spLatest->GetNextObj();
		}
		return true;
	}
};
class CollisonManager
{
public:
	
	static CollisonManager* GetInstance();
	inline void AddCollider(BaseCollider* collider) { colliders.push_front(collider); }//�ǉ�
	inline void RemoveCollider(BaseCollider* collider) { colliders.remove(collider); }//�폜
	void CheckAllCollisons();//�S�Ă̏Փ˂��`�F�b�N
private:
	CollisonManager() = default;
	CollisonManager(const CollisonManager&) = delete;
	~CollisonManager() = default;
	CollisonManager& operator=(const CollisonManager&) = delete;
	//�R���C�_�[�̃��X�g
	forward_list<BaseCollider*>colliders;
	BaseObject* obj;
};
