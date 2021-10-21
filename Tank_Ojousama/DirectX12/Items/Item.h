#pragma once
#include "../Actor/ObjectManager.h"
#include "../Actor/BaseObject.h"
#include "../Render/ModelRenderer.h"

enum ItemNames//アイテム判別用
{
	heal,
	dome,
	smock,
};

//アイテムのベースオブジェクト

class Item :public BaseObject
{
public:
	Item();
	~Item();

private:

	void SetActive(bool value);

	virtual ItemNames GetItemNames() { return itemName; }

	ObjectManager* objM;
	shared_ptr<ModelRenderer>itemModel;
	//アイテム種類分け+複数出す用
	int number = 0;
	string name;
	string num;
	string numName;
	ItemNames itemName;//アイテム種類判別用
	//アイテムの使用フラグ
	bool active;
	//アイテムの取得
	bool isGet;
	//生存時間指定用
	int alive;

	//BaseObject を介して継承
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Rend() = 0;
	virtual void ImGuiDebug() = 0;
	virtual void OnCollison(BaseCollider * col) = 0;
};