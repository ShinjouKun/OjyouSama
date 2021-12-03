#pragma once
#include "Item.h"
#include "ItemHolder.h"


class Player;

class Repair :public Item
{
public:
	Repair(const Vector3& pos, const Vector3& ang, ObjectManager* obj,  shared_ptr<ModelRenderer>m, ItemState itemStates, int num, int maxAlive, int addHp);
	~Repair();

private:

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

	void Heal();

private:
	ObjectManager* objM;
	Player* player=nullptr;
	int alive_max;
	int healPoint;
	int saveHP;

	shared_ptr<ModelRenderer>ItemModel;

};