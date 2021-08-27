#include "BreadCrumb.h"
#include "../Collision/SpherCollider.h"

BreadCrumb::BreadCrumb(Vector3 pos, ObjectManager * objManager, shared_ptr<ModelRenderer> modelRender, int num)
	:modelRender(modelRender)
{
	position = pos;
	objectManager = objManager;
	keyNumber = num;
}

BreadCrumb::~BreadCrumb()
{
}

void BreadCrumb::Init()
{
#pragma region ���f��������

	name = "Bread";
	key = to_string(keyNumber);
	keyName = name + key;
	modelRender->AddModel(keyName, "Resouse/Bullet.obj", "Resouse/Bullet.png");

#pragma endregion

	//�̔ԍ��̓o�^
	//SetNumber(keyNumber);
	AiID = keyNumber;
	objType = ObjectType::BREADCRUMB;
	death = false;
	scale = Vector3(1, 1, 1);
	radius = 1.0f;
	SetCollidder(new SphereCollider(position, radius));

	lifeCount = 0;
	lifeTime = 3 * 60;

	//�p�������Ɏ�����t����I
	//����炢�����G���A�ɓ���Ȃ��悤�ɂ���B
	//�Ԋu���t����
}

void BreadCrumb::Update()
{
	ImGuiDebug();

	if (!death)
	{
		lifeCount++;
		if (lifeCount > lifeTime)
		{
			lifeCount = 0;
			death = true;
		}
	}
}

void BreadCrumb::Rend()
{
	//DirectXManager::GetInstance()->SetData3D();
	//modelRender->Draw(keyName, position, angle, scale);
}

void BreadCrumb::OnCollison(BaseCollider* info)
{
	//if (info.object->GetType() == ObjectType::ENEMY)
	//{
	//	death = true;
	//}
}

void BreadCrumb::ImGuiDebug()
{
	////�_(�v���C���[�ƂȂ����)���̉���
	//float pos[3] = { position.x ,position.y ,position.z };
	//ImGui::SliderFloat3("PointPosition", pos, -500.0f, 500.0f);
	////��Ɠ_���������Ă���`�F�b�N������B
	//ImGui::Checkbox("IsDeath", &death);
}
