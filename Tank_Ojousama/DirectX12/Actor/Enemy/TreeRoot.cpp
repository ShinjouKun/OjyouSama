#include "TreeRoot.h"
#include "../../Collision/BaseCollider.h"

TreeRoot::TreeRoot(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager, 
	std::shared_ptr<ModelRenderer> modelRender,
	int num
)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	mModelRender = modelRender;
	number = num;
}

TreeRoot::~TreeRoot()
{
}

void TreeRoot::Init()
{
	death = false;
	objType = ObjectType::ENEMY;
	damage = 10;

	mScale = Vector3(2.0f, 2.0f, 2.0f);

	SetCollidder(Vector3().zero, mScale.x);

	mModelNum = to_string(number);
	mRootNum = mRoot + mModelNum;
	mModelRender->AddModel(mRootNum, "Resouse/EnemyModel/TreeRoot/nekko.obj", "Resouse/EnemyModel/TreeRoot/nekko.png");
}

void TreeRoot::Update()
{
}

void TreeRoot::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(mRootNum, position, angle, mScale);
}

void TreeRoot::OnCollison(BaseCollider * col)
{
}

void TreeRoot::ImGuiDebug()
{
}
