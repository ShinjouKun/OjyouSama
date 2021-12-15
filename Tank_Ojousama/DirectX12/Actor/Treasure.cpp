#include "Treasure.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"
#include"../Scene/BaseScene.h"
#include"../Utility/Sequence/Sequence.h"
Treasure::Treasure(const Vector3 & pos, const Vector3 & ang, ObjectManager * objManager, std::shared_ptr<ModelRenderer> modelRender,int s)
	:mModelRender(modelRender)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	sceneNum = s;
}

Treasure::~Treasure()
{
}

void Treasure::Init()
{
	SetActive(false);
	death = false;
	get = false;
	angle = Vector3(0.0f,0.0f,0.0f);
	objType = ObjectType::TREASURE;
	if (sceneNum != 1)
	{
		scale = Vector3(1.0f, 1.0f, 1.0f);
	}
	else
	{
		scale = Vector3(3.0f, 3.0f, 3.0f);//強奪用
	}

	mModelRender->AddModel("Treasure","Resouse/boxs.obj","Resouse/boxs.png");
	mModelRender->AddModel("TreasureHuta","Resouse/boxs_huta.obj","Resouse/boxs.png");
	//mModelRender->SetAncPoint("TreasureHuta", Vector3(0.0f, 1.0f, 0.7f));
	numPos = Vector2(350, 250);
	SetCollidder(Vector3(0, 0, 0), 2.0f);
}

void Treasure::Update()
{

}

void Treasure::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	if (get&&sceneNum != 1)
	{
		numPos.y -= 15.0f;
		Sequence::instance().set(500000, Vector2(numPos.x, numPos.y), Vector2(32, 32));
	}

	if (get&&sceneNum == 1)
	{
		numPos.y -= 15.0f;
		Sequence::instance().set(500000, Vector2(numPos.x, numPos.y), Vector2(44, 44));
		angle.y += 2.0f;
		scale = Vector3(0.2f, 0.2f, 0.2f);
		position = mObjManager->GetPlayer().GetPosition();
		mModelRender->Draw("Treasure", Vector3(position.x,4.5f,position.z), angle, scale);
		mModelRender->Draw("TreasureHuta", Vector3(position.x, 4.5f, position.z), angle, scale);
	
	}
	else
	{
		mModelRender->Draw("Treasure", position, angle, scale);
		mModelRender->Draw("TreasureHuta", position, Vector3(angle.x, angle.y,angle.z), scale);
	}

}

void Treasure::ImGuiDebug()
{
}

void Treasure::OnCollison(BaseCollider * col)
{
	if (!get&&col->GetColObject()->GetType() == ObjectType::PLAYER)
	{
		BaseScene::mPlusMoney += 500000;
		get = true;
	}
}
