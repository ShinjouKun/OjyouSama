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

	void Draw();//描画処理

	std::vector<BaseObject*>getUseList();

	//「1=Pキーで落とす」「2=距離で落とす」「3=時間で落とす」
	void DropBreadCrumb(int status);

	BaseObject& GetPlayer()const;

private:
	std::vector<BaseObject*>objectList;//使うもの
	std::vector<BaseObject*>addObjectList;//追加したものの一時保存用
	int IdCount;

	//パンくず距離測定用マップ
	std::unordered_map<int, Vector3> measureMap;
	//パンくずの識別番号
	int breadNumber = 0;
	//パンくずを落とす間隔
	int intervalCount = 0;
	int intervalTime = 1;
};
