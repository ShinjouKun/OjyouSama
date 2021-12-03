#include "Block.h"
#include "../Collision/BaseCollider.h"
Block::Block(const Vector3& pos, const Vector3& ang, ObjectManager * objManager, shared_ptr<ModelRenderer> modelRender, int num)
	:mModelRender(modelRender)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	number = num;
}

Block::~Block()
{
}

void Block::Init()
{
	death = false;
	objType = ObjectType::DEFENCEPOINT;
	angle = Vector3(0, 90, 0);

	name = "Block";
	num = to_string(number);
	numName = name + num;
	scale = Vector3(2.0f, 2.0f, 2.0f);
	radius = 1.0f;

	//コライダーの情報をセット
	//SetCollidder(Vector3().zero, radius);
	SetCollidder(Vector3(-150.0f, -2.0f, -2.0f), Vector3(150.0f, 2.0f, 1.0f));

	mModelRender->AddModel(numName, "Resouse/bill.obj", "Resouse/bill.png");
	mModelRender->SetAncPoint(numName, -scale * 2.0f);//ここは約半径の二倍ずらす
}

void Block::Update()
{

}

void Block::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(numName, position, Vector3(angle.x, angle.y, /*rot += mRotValue*/0), scale);
}

void Block::OnCollison(BaseCollider* col)
{
}

void Block::ImGuiDebug()
{
}
