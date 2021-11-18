#pragma once
#include "../Actor/ObjectManager.h"
#include "../Actor/BaseObject.h"
#include "../Render/ModelRenderer.h"
#include "ItemHolder.h"

class ItemBox: public BaseObject
{
public:
	ItemBox(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, ItemHolder* holder);
	~ItemBox();

private:

	void Init() override;
	void Update() override;
	void Rend() override;
};