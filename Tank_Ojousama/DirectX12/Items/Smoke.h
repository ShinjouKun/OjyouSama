#pragma once
#include "Item.h"
#include "ItemHolder.h"

class Player;

class Smoke :public Item
{
public:
	Smoke(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, ItemState itemStates, int num, int maxAlive, int addHp);
	~Smoke();

private:

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

	void Smoker();

private:
	ObjectManager* objM;
	Player* player = nullptr;
	int alive_max;
	int smokePoint;
	int saveHP;

	bool IsBlind;

	shared_ptr<ModelRenderer>ItemModel;

};