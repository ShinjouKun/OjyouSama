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
 class CollisonTree :public enable_shared_from_this<CollisonTree>//当たり判定文木登録クラス
{
public:
	WorldSpace *wSpace;//登録空間
	BaseCollider *colObject;//判定対象
	CollisonTree* m_spPre;
	CollisonTree* m_spNext;
public:
	CollisonTree();
	virtual ~CollisonTree();

public:
	//リストからの除外
	bool Remove();
	//空間を登録
	void RegistWorld(WorldSpace *wSpa);
	//次のオブジェクトへのスマートポインタを確保
	CollisonTree *GetNextObj();
};