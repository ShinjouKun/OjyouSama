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
	inline void AddCollider(BaseCollider* collider) { colliders.push_front(collider); }//追加
	inline void RemoveCollider(BaseCollider* collider) { colliders.remove(collider); }//削除
	void CheckAllCollisons();//全ての衝突をチェック
private:
	CollisonManager() = default;
	CollisonManager(const CollisonManager&) = delete;
	~CollisonManager() = default;
	CollisonManager& operator=(const CollisonManager&) = delete;
	//コライダーのリスト
	forward_list<BaseCollider*>colliders;
	BaseObject* obj;
};
