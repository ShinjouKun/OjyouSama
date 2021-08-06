#pragma once
#include<forward_list>
//#include"BaseObject.h"
#include "../Actor/BaseObject.h"
using namespace std;
class BaseCollider;

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
