#pragma once
#include "Item.h"
#include "ItemHolder.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include <memory>


class Player;
class Sound;

class Repair :public Item
{
public:
	Repair(const Vector3& pos, const Vector3& ang, ObjectManager* obj,  shared_ptr<ModelRenderer>m, shared_ptr<TexRenderer>s, ItemState itemStates, int num, int aliveNum, int addHp);
	~Repair();

private:

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

	void Heal();

	/*ínñ ÇÊÇËçÇÇ¢Ç∆Ç±ÇÎÇ…Ç†Ç¡ÇΩÇÁóéÇøÇƒÇ≠ÇÈ*/
	void FallDown();

private:
	ObjectManager* objM;
	Player* player=nullptr;
	int healPoint;
	int saveHP;
	int fadeCount;
	float itemFade;


	shared_ptr<ModelRenderer>ItemModel;
	shared_ptr<TexRenderer>ItemUseTex;
	std::shared_ptr<Sound>getSE;
	std::shared_ptr<Sound>healSE;

};