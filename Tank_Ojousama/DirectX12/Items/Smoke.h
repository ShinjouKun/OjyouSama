#pragma once
#include "Item.h"
#include "ItemHolder.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Player;
class Sound;

class Smoke :public Item
{
public:
	Smoke(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, ItemState itemStates, int num, int addHp);
	~Smoke();

private:

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

	void Smoker();

	/*ínñ ÇÊÇËçÇÇ¢Ç∆Ç±ÇÎÇ…Ç†Ç¡ÇΩÇÁóéÇøÇƒÇ≠ÇÈ*/
	void FallDown();

private:
	ObjectManager* objM;
	Player* player = nullptr;
	int smokePoint;
	int saveHP;

	bool IsBlind;

	shared_ptr<ModelRenderer>ItemModel;
	std::shared_ptr<Sound>getSE;

};