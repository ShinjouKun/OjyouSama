#pragma once
#include<forward_list>
#include<memory>
#include<vector>
#include <vector>
#include <list>
#include <map>
#include<iterator>
#include"../Utility/SmartPtr.h"
#include "../Math/Vector3.h"
using namespace std;
class BaseObject;
class BaseCollider;
class OctTreeManager;
class WorldSpace;//�����蔻�蕪���p�̋�ԃN���X



class CollisonManager
{
public:
	bool OctTreeSet();//�����،n�̏�������Z�b�g
	static CollisonManager* GetInstance();
	OctTreeManager* GetInstanceOct();//�����؃}�l�[�W���[�̃C���X�^���X�l��
	OctTreeManager *S8Tree;
	inline void AddCollider(BaseCollider* collider) { colliders.push_front(collider); }//�ǉ�
	inline void RemoveCollider(BaseCollider* collider) { colliders.remove(collider); }//�폜
	void CheckAllCollisons();//�S�Ă̏Փ˂��`�F�b�N
private:
	CollisonManager()
	{
		OctTreeSet();
	}
	CollisonManager(const CollisonManager&) = delete;
	~CollisonManager() = default;
	CollisonManager& operator=(const CollisonManager&) = delete;
	//�R���C�_�[�̃��X�g
	forward_list<BaseCollider*>colliders;
	BaseObject* obj;
	
	
};
