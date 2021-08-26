 #pragma once
#include <vector>
#include <list>
#include <map>
#include<memory>
#include<iterator>
#include"../Utility/SmartPtr.h"
#include "../Math/Vector3.h"

using namespace std;
//線形八分木空間管理クラス
#define TREEMANAGER_MAXLEVEL 7
class CollisonTree;
class BaseCollider;
class WorldSpace;
class OctTreeManager
{
protected:
	unsigned int m_uiDim;
	WorldSpace **ppWsAry;//線形空間ポインタ配列
	unsigned int m_iPow[TREEMANAGER_MAXLEVEL + 1];//べき乗数値配列
	Vector3 m_W;//領域の幅
	Vector3 m_RgnMin;//領域の最小値
	Vector3 m_RgnMax;//領域の最大値
	Vector3 m_Unit;//最小領域の辺の長さ
	DWORD m_dwWorldNum;//空間の数
	unsigned int m_uiLevel;//最下レベル

public:
	//コンストラクタ
	OctTreeManager();
	//デストラクタ
	virtual ~OctTreeManager();
	//線形八分木の構築
	bool Init(unsigned int Level, const Vector3&Min, const Vector3&Max);
	
	//オブジェクトの登録
	bool Regist(Vector3* Min, Vector3* Max, CollisonTree* spOFT);

	//衝突判定リストの作成
	DWORD GetAllCollisonList(vector<BaseCollider*> &ColVect);

	// 空間内で衝突リストを作成
	bool GetCollisonList(DWORD Elem, vector<BaseCollider*> &ColVect, list<BaseCollider*> &ColStac);
	
	//空間の生成
	bool CreateNewWorld(DWORD Elem);
	//座標から空間番号を算出
	DWORD GetMortonNumber(Vector3* Min, Vector3* Max);
	DWORD BitSeparateFor3D(BYTE n);
	DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z);
	DWORD GetPointElem(const Vector3& p);
};