#include"../DirectX12/Collision/CollisonTree.h"
#include"../DirectX12/Collision/WorldSpace.h"
CollisonTree::CollisonTree()
{
	wSpace = NULL;
	colObject = NULL;
	m_spNext = nullptr;
	m_spPre = nullptr;
}

CollisonTree::~CollisonTree()
{
	Remove();
}

bool CollisonTree::Remove()
{
	if (!wSpace)//すでに逸脱している場合
		return false;
	
	if (!wSpace->OnRemove(this))//自分を登録している空間に自身を通知
		return false;
	//逸脱処理
	//前後のオブジェクトを結びつける
	if (m_spPre != nullptr)
	{
		m_spPre->m_spNext = m_spNext;
	}
	if (m_spNext != nullptr)
	{
		m_spNext->m_spPre = m_spPre;
	}
	m_spNext = nullptr;
	m_spPre = nullptr;
	wSpace = NULL;
	return true;
}

void CollisonTree::RegistWorld(WorldSpace *wSpa)
{
	wSpace = wSpa;
}

CollisonTree* CollisonTree::GetNextObj()
{
	return m_spNext;
}
