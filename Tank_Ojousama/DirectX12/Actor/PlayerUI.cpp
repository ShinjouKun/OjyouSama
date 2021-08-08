#include "PlayerUI.h"


PlayerUI::PlayerUI(ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<TexRenderer> s)
	:UIModel(m),UISprite(s)
{
	objM = obj;
	Init();
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Init()
{
	death = false;
	UIModel->AddModel("AIM", "Resouse/Plane.obj", "Resouse/AIM64.png");
	AimPos = Vector3(100.0f,0.0f,-50.0f);
}

void PlayerUI::Update()
{
}

void PlayerUI::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	UIModel->Draw("AIM", Vector3(AimPos.x, AimPos.y, AimPos.z), Vector3(-90.0f, 0.0f, 0), Vector3(0.5f, 0.5f, 0.5f));
}

void PlayerUI::ImGuiDebug()
{
}

void PlayerUI::OnCollison(const CollisonInfo & info)
{
}
