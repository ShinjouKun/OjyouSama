#pragma once
#include<vector>
#include<memory>
#include"BaseObject.h"
#include "../Device/DirectXManager.h"
#include "../Collision/Collision.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	void Claer();//各リストの初期化
	void Add(BaseObject* addObj);//リストへの追加
	void Update();//総更新
	void AddListUpdate();//追加したリストの更新
	void ObjectManagerUpdate();//リスト内のオブジェクトの更新
	void RemoveListUpdate();//死んだものなどの更新処理
	int GetEnemyDeath() { return EnemyDeath; }
	void SetEnemyDeath(int ed) { EnemyDeath = ed; }
	int GetReloadTime() { return ReloadTime; }
	void SetReloadTime(int time) { ReloadTime = time; }
	void Draw();//描画処理
	std::vector<BaseObject*>getUseList();

	BaseObject & GetPlayer() const;
	BaseObject & GetGolem() const;
	BaseObject & GetEnemy() const;
	vector<BaseObject*>GetEnemyList()const;
	BaseObject & GetCastle()const;
private:
	std::vector<BaseObject*>objectList;//使うもの
	std::vector<BaseObject*>addObjectList;//追加したものの一時保存用
	int EnemyDeath;
	int IdCount;
	int ReloadTime;
};
