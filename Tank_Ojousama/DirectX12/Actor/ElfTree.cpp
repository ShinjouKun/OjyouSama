#include "ElfTree.h"

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
	death = false;
	objType = ObjectType::BLOCK;

	name = "ElfTree";
	num = to_string(number);
	numName = name + num;
	scale = Vector3(2.0f, 2.0f, 2.0f);
	radius = 1.0f;
	isActive = true;

	//コライダーの情報をセット
	SetCollidder(Vector3(position.x, position.y, position.z), radius);
	mModelRender->AddModel(numName, "Resouse/wood.obj", "Resouse/Big-treeA.png");
}

void ElfTree::Update()
{
	Vector3 distance = mObjManager->GetPlayer().GetPosition() - position;
	float length = distance.Length();

	if (length > 100.0f)
	{
		isActive = false;
	}
	else
	{
		isActive = true;
	}
}

void ElfTree::Rend()
{
	if (!isActive) return;
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(numName, position, angle, scale);
}

void ElfTree::OnCollison(BaseCollider * col)
{
}

void ElfTree::ImGuiDebug()
{
}
