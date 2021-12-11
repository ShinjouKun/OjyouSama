#pragma once
#include "Item.h"
#include "ItemHolder.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Player;

class Shield :public Item
{
public:
	Shield(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m,shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, ItemState itemState, int num, int addHp);
	~Shield();

private:

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

	void Guade();

private:
	ObjectManager* objM;
	Player* player = nullptr;
	int guadePoint;
	int saveHP;
	int damege;

	shared_ptr<ModelRenderer>ItemModel;
};