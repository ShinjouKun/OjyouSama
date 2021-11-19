#pragma once
#include "../Actor/ObjectManager.h"
#include "../Actor/BaseObject.h"
#include "../Render/ModelRenderer.h"
#include "ItemHolder.h"

class ItemBox: public BaseObject
{
public:
	ItemBox(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, ItemHolder* holder,int Number,int itemNumber,int alive,int addHealth);
	~ItemBox();

private:

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

	void ItemSpawn();

private:
	shared_ptr<ModelRenderer>ItemModel;
	ObjectManager* objM;
	ItemHolder* iHolder;
	Vector3 position;
	Vector3 angle;
	string name;
	string num;
	string numName;
	int itemNum;
	int itemAlive;
	int addHp;
	int nomber


};