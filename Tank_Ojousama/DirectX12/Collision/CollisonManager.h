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
class WorldSpace;//当たり判定分割用の空間クラス



class CollisonManager
{
public:
	bool OctTreeSet();//八分木系の初期化やセット
	static CollisonManager* GetInstance();
	OctTreeManager* GetInstanceOct();//八分木マネージャーのインスタンス獲得
	OctTreeManager *S8Tree;
	inline void AddCollider(BaseCollider* collider) { colliders.push_front(collider); }//追加
	inline void RemoveCollider(BaseCollider* collider) { colliders.remove(collider); }//削除
	void CheckAllCollisons();//全ての衝突をチェック
private:
	CollisonManager()
	{
		OctTreeSet();
	}
	CollisonManager(const CollisonManager&) = delete;
	~CollisonManager() = default;
	CollisonManager& operator=(const CollisonManager&) = delete;
	//コライダーのリスト
	forward_list<BaseCollider*>colliders;
	BaseObject* obj;
	
	
};
