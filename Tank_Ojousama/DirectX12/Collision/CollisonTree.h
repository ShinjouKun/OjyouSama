#pragma once
#include<forward_list>
#include<vector>
#include <vector>
#include <list>
#include <map>
#include<iterator>
#include"../Utility/SmartPtr.h"
#include "../Math/Vector3.h"
#include <memory>

using namespace std;
class BaseCollider;
class WorldSpace;
 class CollisonTree :public enable_shared_from_this<CollisonTree>//�����蔻�蕶�ؓo�^�N���X
{
public:
	WorldSpace *wSpace;//�o�^���
	BaseCollider *colObject;//����Ώ�
	CollisonTree* m_spPre;
	CollisonTree* m_spNext;
public:
	CollisonTree();
	virtual ~CollisonTree();

public:
	//���X�g����̏��O
	bool Remove();
	//��Ԃ�o�^
	void RegistWorld(WorldSpace *wSpa);
	//���̃I�u�W�F�N�g�ւ̃X�}�[�g�|�C���^���m��
	CollisonTree *GetNextObj();
};