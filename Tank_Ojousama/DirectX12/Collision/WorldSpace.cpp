#include "WorldSpace.h"
#include "CollisonTree.h"
WorldSpace::WorldSpace()
{
	m_spLatest = nullptr;
}

void WorldSpace::ResetLink(CollisonTree* sp0FT)
{
	if (sp0FT->m_spNext != nullptr)
		ResetLink(sp0FT->m_spNext);
	sp0FT = nullptr;//スマートポインタリセット
}

bool WorldSpace::Push(CollisonTree* sp0FT)
{
	if (sp0FT == nullptr)return false;//無効なオブジェクトは登録しない
	if (sp0FT->wSpace == this)return false;//二重登録をチェック
	if (m_spLatest == nullptr)
	{
		m_spLatest = sp0FT;//空間に新規登録
	}
	else
	{
		//最新0FTオブジェクトを更新
		sp0FT->m_spNext = m_spLatest;
		m_spLatest->m_spPre = sp0FT;
		m_spLatest = sp0FT;
	}
	sp0FT->RegistWorld(this);//空間を登録
	return true;
}

CollisonTree* WorldSpace::GetFirstObj()
{
	return m_spLatest;
}

bool WorldSpace::OnRemove(CollisonTree *pRemoveObj)
{
	if (m_spLatest == pRemoveObj)
	{
		//次のオブジェクトに挿げ替え
		if (m_spLatest != nullptr)
			m_spLatest = m_spLatest->GetNextObj();
	}
	return true;
}
