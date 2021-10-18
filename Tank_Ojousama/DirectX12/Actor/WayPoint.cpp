#include "WayPoint.h"

WayPoint::WayPoint(const Vector3 & pos, ObjectManager * objManager, shared_ptr<ModelRenderer> modelRender, int num)
	:modelRenderer(modelRender)
{
	position = pos;
	objectManager = objManager;
	keyNumber = num;

	FlagInit();
}

WayPoint::WayPoint()
{
	FlagInit();
}

WayPoint::~WayPoint()
{
}

void WayPoint::FlagInit()
{
	//�e�t���O�̏�����
	useFlag = false;
	arrivalFlag = false;
	impossibleFlag = false;
	closeFlag = false;
}

void WayPoint::Init()
{
	FlagInit();

#pragma region ���f���̏�����

	name = "WayPoint";
	key = to_string(keyNumber);
	keyName = name + key;
	modelRenderer->AddModel(keyName, "Resouse/Bullet.obj", "Resouse/Bullet.png");

#pragma endregion

	//�̔ԍ��̓o�^
	AiID = keyNumber;
	//objType = ObjectType::BREADCRUMB;
	death = false;

}

void WayPoint::Update()
{
	//ImGuiDebug();
	//ImGui::Text("WayPoint");
}

void WayPoint::Rend()
{
	DirectXManager::GetInstance()->SetData3D();
	modelRenderer->Draw(keyName, position, angle, Vector3(1,1,1));
}

void WayPoint::OnCollison(BaseCollider * col)
{
}

void WayPoint::ImGuiDebug()
{
	//�_(�v���C���[�ƂȂ����)���̉���
	float pos[3] = { position.x ,position.y ,position.z };
	ImGui::SliderFloat3("Position", pos, -1000.0f, 1000.0f);
	ImGui::SliderInt("WayPoint", &keyNumber, 0, 100);
}
