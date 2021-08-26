#include"OctTreeManager.h"
#include"../Collision/WorldSpace.h"
#include"../Collision/CollisonTree.h"
#include"../Collision/WorldSpace.h"
OctTreeManager::OctTreeManager()
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

OctTreeManager::~OctTreeManager()
{

	DWORD i;
	for (i = 0; i < m_dwWorldNum; i++)
	{
		if (ppWsAry[i] != NULL)
			delete ppWsAry[i];
	}
	delete[] ppWsAry;

}

bool OctTreeManager::Init(unsigned int Level, const Vector3 & Min, const Vector3 & Max)
{
	// 設定最高レベル以上の空間は作れない
	if (Level >= TREEMANAGER_MAXLEVEL)
		return false;
	// 各レベルでの空間数を算出
	int i;
	m_iPow[0] = 1;
	for (i = 1; i < TREEMANAGER_MAXLEVEL + 1; i++)
		m_iPow[i] = m_iPow[i - 1] * 8;
	// Levelレベル（0基点）の配列作成
	m_dwWorldNum = (m_iPow[Level + 1] - 1) / 7;
	ppWsAry = new WorldSpace*[m_dwWorldNum];
	ZeroMemory(ppWsAry, sizeof(WorldSpace*)*m_dwWorldNum);

	//領域の登録
	m_RgnMin = Min;
	m_RgnMax = Max;
	m_W = m_RgnMax - m_RgnMin;
	m_Unit = m_W / ((float)(1 << Level));

	m_uiLevel = Level;

	return true;
}

bool OctTreeManager::Regist(Vector3 * Min, Vector3 * Max, CollisonTree* spOFT)
{
	// オブジェクトの境界範囲から登録モートン番号を算出
	DWORD Elem = GetMortonNumber(Min, Max);
	if (Elem < m_dwWorldNum)
	{
		//空間がない場合は新しく作る
		if (!ppWsAry[Elem])
			CreateNewWorld(Elem);
		return ppWsAry[Elem]->Push(spOFT);
	}
	return false;//登録失敗
}

DWORD OctTreeManager::GetAllCollisonList(vector<BaseCollider*>& ColVect)
{
	//初期化
	ColVect.clear();
	//ルート空間の存在を確認
	if (ppWsAry[0] == NULL)
		return 0;//何もない
	//ルート空間の処理
	list<BaseCollider*>ColStac;
	GetCollisonList(0, ColVect, ColStac);

	return (DWORD)ColVect.size();
}

bool OctTreeManager::GetCollisonList(DWORD Elem, vector<BaseCollider*>& ColVect, list<BaseCollider*>& ColStac)
{
	typename list<BaseCollider*>::iterator it;
	//空間内のオブジェクト同士の衝突リスト作成
	CollisonTree* sp0FT1 = ppWsAry[Elem]->GetFirstObj();
	while (sp0FT1 != nullptr)
	{
		CollisonTree* sp0FT2 = sp0FT1->m_spNext;
		while (sp0FT2 != NULL)
		{
			//衝突リスト作成
			ColVect.push_back(sp0FT1->colObject);
			ColVect.push_back(sp0FT2->colObject);
			sp0FT2 = sp0FT2->m_spNext;
		}
		//衝突スタックとの衝突リスト作成
		for (it = ColStac.begin(); it != ColStac.end(); it++)
		{
			ColVect.push_back(sp0FT1->colObject);
			ColVect.push_back(*it);
		}
		sp0FT1 = sp0FT1->m_spNext;
	}
	bool ChildFlag = false;
	//子空間に移動
	DWORD objNum = 0;
	DWORD i, NextElem;
	for (i = 0; i < 8; i++)
	{
		NextElem = Elem * 8 + 1 + i;
		if (NextElem < m_dwWorldNum && ppWsAry[Elem * 8 + 1 + i])
		{
			if (!ChildFlag)
			{
				//登録オブジェクトをスタックに追加
				sp0FT1 = ppWsAry[Elem]->GetFirstObj();
				while (sp0FT1)
				{
					ColStac.push_back(sp0FT1->colObject);
					objNum++;
					sp0FT1 = sp0FT1->m_spNext;
				}
			}
			ChildFlag = true;
			GetCollisonList(Elem * 8 + 1 + i, ColVect, ColStac);//子空間へ
		}
	}
	//スタックからオブジェクトを外す
	if (ChildFlag)
	{
		for (i = 0; i < objNum; i++)
		{
			ColStac.pop_back();
	    }
	}
	return true;
}

bool OctTreeManager::CreateNewWorld(DWORD Elem)
{
	//引数の要素番号
	while (!ppWsAry[Elem])
	{
		ppWsAry[Elem] = new WorldSpace;

		//親空間にジャンプ
		Elem = (Elem - 1) >> 3;
		if (Elem >= m_dwWorldNum)break;
	}
	return true;
}

DWORD OctTreeManager::GetMortonNumber(Vector3 * Min, Vector3 * Max)
{
	// 最小レベルにおける各軸位置を算出
	DWORD LT = GetPointElem(*Min);
	DWORD RB = GetPointElem(*Max);
	// 空間番号を引き算して最上位区切りから所属レベルを算出
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

DWORD OctTreeManager::BitSeparateFor3D(BYTE n)
{
	DWORD s = n;
	s = (s | s << 8) & 0x0000f00f;
	s = (s | s << 4) & 0x000c30c3;
	s = (s | s << 2) & 0x00249249;
	return s;
}

DWORD OctTreeManager::Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
{
	return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
}

DWORD OctTreeManager::GetPointElem(const Vector3 & p)
{
	return Get3DMortonNumber(
		(BYTE)((p.x - m_RgnMin.x) / m_Unit.x),
		(BYTE)((p.y - m_RgnMin.y) / m_Unit.y),
		(BYTE)((p.z - m_RgnMin.z) / m_Unit.z)
	);
}
