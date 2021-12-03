#include "AdvanceBorderLine.h"
#include "../../Collision/BaseCollider.h"
#include "../../Collision/SpherCollider.h"


AdvanceBorderLine::AdvanceBorderLine(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager, 
	shared_ptr<ModelRenderer> modelRender,
	int num, 
	const Vector3& min,
	const Vector3& max
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

AdvanceBorderLine::~AdvanceBorderLine()
{
}

void AdvanceBorderLine::Init()
{
	SetActive(true);
	death = false;
	objType = ObjectType::BORDER_LINE;

	name = "BorderLine";
	num = to_string(number);
	numName = name + num;
	mModelRender->AddModel(numName, "Resouse/Plane.obj", "Resouse/kabe.png");

	//////コライダーの情報をセット
	SetCollidder(mMin, mMax);
	//SetCollidder(Vector3(-52.0f,-2.0f,-2.0f), Vector3(2.0f, 2.0f, 2.0f));
	////SetCollidder(Vector3().zero, 1.2f);
}

void AdvanceBorderLine::Update()
{
	
}

void AdvanceBorderLine::Rend()
{
	DirectXManager::GetInstance()->SetData3D();       //↓大きさは固定値
	mModelRender->Draw(numName, Vector3(position.x, position.y, position.z), angle, Vector3(1, 1, 1));
}

void AdvanceBorderLine::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}
}

void AdvanceBorderLine::ImGuiDebug()
{
}
