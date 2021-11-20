#include "BetaTestBlock.h"
#include "../Collision/BaseCollider.h"

BetaTestBlock::BetaTestBlock(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	shared_ptr<ModelRenderer> modelRender,
	int num,
	const Vector3 & min,
	const Vector3 & max
)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	mModelRender = modelRender;
	number = num;
	mMin = min;
	mMax = max;
}

BetaTestBlock::~BetaTestBlock()
{
}

void BetaTestBlock::Init()
{
	SetActive(true);
	death = false;
	objType = ObjectType::BLOCK;

	name = "KabeKabe";
	num = to_string(number);
	numName = name + num;
	mModelRender->AddModel(numName, "Resouse/Plane.obj", "Resouse/kabe.png");

	//コライダーの情報をセット
	SetCollidder(mMin, mMax);
	//SetCollidder(Vector3(0,0,0), radius);
}

void BetaTestBlock::Update()
{
}

void BetaTestBlock::Rend()
{
	DirectXManager::GetInstance()->SetData3D();       //↓大きさは固定値
	mModelRender->Draw(numName, Vector3(position.x,position.y + 60.0f,position.z), angle, Vector3(300, 80, 300));
}

void BetaTestBlock::OnCollison(BaseCollider * col)
{
	//if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	//{
	//	SetActive(true);
	//}
}

void BetaTestBlock::ImGuiDebug()
{
}
