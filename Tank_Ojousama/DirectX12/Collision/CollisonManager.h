#pragma once
#include<forward_list>
#include<vector>
#include <vector>
#include <list>
#include <map>
#include<iterator>
//#include"BaseObject.h"
#include "../Actor/BaseObject.h"
#include"../Utility/SmartPtr.h"
using namespace std;
class BaseCollider;
template<class  T>
class WorldSpace;//当たり判定分割用の空間クラス
template<class  T>
class CollisonTree//当たり判定文木登録クラス
{
public:
	WorldSpace<T> *wSpace;//登録空間
	T *colObject;//判定対象
	sp<CollisonTree<T>>m_spPre;//前のTree構造体
	sp<CollisonTree<T>>m_spNext;//次のTree構造体
public:
	CollisonTree()
	{
		wSpace = NULL;
		colObject = NULL;
	}

	virtual ~CollisonTree(){}

public:
	//リストからの除外
	bool Remove()
	{
		if (!wSpace)//すでに逸脱している場合
			return false;
		if (!wSpace->OnRemove(this))//自分を登録している空間に自身を通知
			return false;
		//逸脱処理
		//前後のオブジェクトを結びつける
		if (m_spPre.GetPtr() != NULL)
		{
			m_spPre->m_spNext = m_spNext;
			m_spPre.SetPtr(NULL);
		}
		if (m_spNext.GetPtr() != NULL)
		{
			m_spNext->m_spPre = m_spPre;
			m_spNext.SetPtr(NULL);
		}
		wSpace = NULL;
		return true;
	}
	//空間を登録
	void RegistWorld(WorldSpace<T> *wSpa)
	{
		wSpace = wSpa;
	}
	//次のオブジェクトへのスマートポインタを確保
	sp<CollisonTree<T>>&GetNextObj() { return m_spNext; }
};
//線形八分木空間管理クラス
#define TREEMANAGER_MAXLEVEL 7
template<class T>
class OctTreeManager
{
protected:
	unsigned int m_uiDim;
	WorldSpace<T> **ppWsAry;//線形空間ポインタ配列
	unsigned int m_iPow[TREEMANAGER_MAXLEVEL + 1];//べき乗数値配列
	Vector3 m_W;//領域の幅
	Vector3 m_RgnMin;//領域の最小値
	Vector3 m_RgnMax;//領域の最大値
	Vector3 m_Unit;//最小領域の辺の長さ
	DWORD m_dwWorldNum;//空間の数
	unsigned int m_uiLevel;//最下レベル
public:
	//コンストラクタ
	OctTreeManager()
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
	//デストラクタ
	virtual ~OctTreeManager()
	{
		DWORD i;
		for (i = 0; i < m_dwWorldNum; i++)
		{
			if (ppWsAry[i] != NULL)
				delete ppWsAry[i];
		}
		delete[] ppWsAry;
	}
	//線形八分木の構築
	bool Init(unsigned int Level, const Vector3&Min, const Vector3&Max)
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
		ppWsAry = new WorldSpace<T>*[m_dwWorldNum];
		ZeroMemory(ppWsAry, sizeof(WorldSpace<T>*)*m_dwWorldNum);

		//領域の登録
		m_RgnMin = Min;
		m_RgnMax = Max;
		m_W = m_RgnMax - m_RgnMin;
		m_Unit = m_W / ((float)(1 << Level));

		m_uiLevel = Level;

		return true;
	}
	//オブジェクトの登録
	bool Regist(Vector3* Min, Vector3* Max, sp<CollisonTree<T>>&spOFT)
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
	//衝突判定リストの作成
	DWORD GetAllCollisonList(vector<T*> &ColVect)
	{
		//初期化
		ColVect.clear();
		//ルート空間の存在を確認
		if (ppWsAry[0] == NULL)
			return 0;//何もない
		//ルート空間の処理
		list<T*>ColStac;
		GetCollisonList(0, ColVect, ColStac);

		return (DWORD)ColVect.size();
	}
protected:
	// 空間内で衝突リストを作成
	bool GetCollisonList(DWORD Elem, vector<T*> &ColVect, list<T*> &ColStac)
	{
		typename list<T*>::iterator it;
		//空間内のオブジェクト同士の衝突リスト作成
		sp<CollisonTree<T>> sp0FT1 = ppWsAry[Elem]->GetFirstObj();
		while (sp0FT1.GetPtr()!=NULL)
		{
			sp<CollisonTree<T>>sp0FT2 = sp0FT1->m_spNext;
			while (sp0FT2 !=NULL)
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
					while (sp0FT1.GetPtr())
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
			for (i=0;i<objNum;i++)
			{
				ColStac.pop_back();
			}
			return true;
		}

	}
	//空間の生成
	bool CreateNewWorld(DWORD Elem)
	{
		//引数の要素番号
		while (!ppWsAry[Elem])
		{
			ppWsAry[Elem] = new WorldSpace<T>;

			//親空間にジャンプ
			Elem = (Elem - 1) >> 3;
			if (Elem >= m_dwWorldNum)break;
		}
		return true;
	}
	//座標から空間番号を算出
	DWORD GetMortonNumber(Vector3* Min, Vector3* Max)
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
	// ビット分割関数
	DWORD BitSeparateFor3D(BYTE n)
	{
		DWORD s = n;
		s = (s | s << 8) & 0x0000f00f;
		s = (s | s << 4) & 0x000c30c3;
		s = (s | s << 2) & 0x00249249;
		return s;
	}

	// 3Dモートン空間番号算出関数
	DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
	{
		return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
	}

	// 座標→線形8分木要素番号変換関数
	DWORD GetPointElem(const Vector3& p)
	{
		return Get3DMortonNumber(
			(BYTE)((p.x - m_RgnMin.x) / m_Unit.x),
			(BYTE)((p.y - m_RgnMin.y) / m_Unit.y),
			(BYTE)((p.z - m_RgnMin.z) / m_Unit.z)
		);
	}
};
//空間クラス
template<class T>
class WorldSpace
{
protected:
	sp<CollisonTree<T>>m_spLatest;//最新OFTへのスマートポインタ
public:
	WorldSpace() 
	{
	}
	virtual ~WorldSpace()
	{
		if (m_spLatest.GetPtr() != NULL)
		ResetLink(m_spLatest);
	}
	//リンクを全てリセットする
	void ResetLink(sp<CollisonTree<T>> &sp0FT)
	{
		if (sp0FT->m_spNext.GetPtr()!= NULL)
			ResetLink(sp0FT->m_spNext);
		sp0FT.SetPtr(NULL);//スマートポインタリセット
	}
	//0FTをプッシュ
	bool Push(sp<CollisonTree<T>> &sp0FT)
	{
		if (sp0FT.GetPtr() == NULL)return false;//無効なオブジェクトは登録しない
		if (sp0FT->wSpace == this)return false;//二重登録をチェック
		if (m_spLatest.GetPtr() == NULL)
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
	sp<CollisonTree<T>>&GetFirestObj()
	{
		return m_spLatest;
	}
	//削除されるオブジェクトのチェック
	bool OnRemove(CollisonTree<T> *pRemoveObj)
	{
		if (m_spLatest.GetPtr() == pRemoveObj)
		{
			//次のオブジェクトに挿げ替え
			if (m_spLatest.GetPtr() != NULL)
				m_spLatest = m_spLatest->GetNextObj();
		}
		return true;
	}
};
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
