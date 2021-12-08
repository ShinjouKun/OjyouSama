#include "Treasure.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"
Treasure::Treasure(const Vector3 & pos, const Vector3 & ang, ObjectManager * objManager, std::shared_ptr<ModelRenderer> modelRender)
	:mModelRender(modelRender)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
}

Treasure::~Treasure()
{
}

void Treasure::Init()
{
	SetActive(false);
	death = false;
	get = false;
	objType = ObjectType::TREASURE;
	scale = Vector3(1.0f, 1.0f, 1.0f);
	mModelRender->AddModel("Treasure","Resouse/boxs.obj","Resouse/boxs.png");
	mModelRender->AddModel("TreasureHuta","Resouse/boxs_huta.obj","Resouse/boxs.png");

	SetCollidder(Vector3(0, 0, 0), 2.0f);
}

void Treasure::Update()
{

}

void Treasure::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	if (get)
	{
		scale = Vector3(0.2f, 0.2f, 0.2f);
		position = mObjManager->GetPlayer().GetPosition();
		mModelRender->Draw("Treasure", Vector3(position.x,4.5f,position.z), angle, scale);
		mModelRender->Draw("TreasureHuta", Vector3(position.x, 4.5f, position.z), angle, scale);
	
	}
	else
	{
		mModelRender->Draw("Treasure", position, angle, scale);
		mModelRender->Draw("TreasureHuta", position, angle, scale);
	}

}

void Treasure::ImGuiDebug()
{
}

void Treasure::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::PLAYER)
	{
		get = true;
	}
}
