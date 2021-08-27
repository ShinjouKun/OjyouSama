#pragma once
#include <vector>
#include <list>
#include <map>
#include<iterator>
#include"../Utility/SmartPtr.h"
#include "../Math/Vector3.h"
#include<memory>

using namespace std;
//空間クラス
class BaseCollider;
class CollisonTree;
class WorldSpace
{
private:
	CollisonTree* m_spLatest;//最新OFTへのスマートポインタ
public:
	WorldSpace();
	virtual ~WorldSpace()
	{
		if (m_spLatest != nullptr)
			ResetLink(m_spLatest);
	}
	void ResetLink(CollisonTree *sp0FT);
	bool Push(CollisonTree *sp0FT);
	CollisonTree* GetFirstObj();
	bool OnRemove(CollisonTree *pRemoveObj);
};