#include "Block.h"

Block::Block(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, int n)
	:blockModel(m)
{
	position = pos;
	angle = ang;
	objM = obj;
	number = n;
}

Block::~Block()
{
}

void Block::Init()
{
	death = false;
	objType = ObjectType::BLOCK;
	name = "Block";
	num = to_string(number);
	numName = name + num;
	angle = Vector3(0, 0, 0);
	
	blockModel->AddModel(numName, "Resouse/bill.obj", "Resouse/bill.png");
	blockModel->SetAncPoint(numName, Vector3(-1.0f,-2.0f, -3.0f));
}

void Block::Update()
{
	
}

void Block::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	blockModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), Vector3(2, 2, 2));
}

void Block::OnCollison(const CollisonInfo & info)
{
}

void Block::ImGuiDebug()
{
}
