#include "ElfTree.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"
ElfTree::ElfTree(const Vector3 & pos, const Vector3 & ang, ObjectManager* objManager, std::shared_ptr<ModelRenderer> modelRender, int num)
	:mModelRender(modelRender)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	number = num;
}

ElfTree::~ElfTree()
{
}

void ElfTree::Init()
{
	SetActive(false);
	death = false;
	objType = ObjectType::BLOCK;

	name = "ElfTree";
	num = to_string(number);
	numName = name + num;
	scale = Vector3(2.0f, 2.0f, 2.0f);
	radius = 1.2f;
	
	
	mModelRender->AddModel(numName, "Resouse/wood.obj", "Resouse/Big-treeA.png");
	mModelRender->SetAncPoint(numName, Vector3(0.0f, -2.0f, 0.0f));
	//コライダーの情報をセット
	SetCollidder(Vector3(0,0,0), radius);
}

void ElfTree::Update()
{
	//ImGui::Checkbox("TreeActive", &IsActive);
}

void ElfTree::Rend()
{
	if (GetActive())
	{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		mModelRender->Draw(numName, position, angle, scale);
	}
	
}

void ElfTree::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}
}

void ElfTree::ImGuiDebug()
{
}
