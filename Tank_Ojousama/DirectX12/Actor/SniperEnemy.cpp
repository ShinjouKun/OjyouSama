#include "SniperEnemy.h"
#include "../Weapons/NormalBullet.h"
#include "../Collision/SpherCollider.h"
#include "../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../ConstInfomation/Enemy/SniperEnemyConstInfo.h"

namespace ECI = EnemyConstInfo;
namespace SECI = SniperEnemyConstInfo;

SniperEnemy::SniperEnemy(
	const Vector3& pos,
	const Vector3& ang,
	ObjectManager * objM,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<TexRenderer> texRender,
	shared_ptr<ParticleManager> effect,
	int num
) : modelRender(modelRender), effectManager(effect)
{
	position = pos;
	angle = ang;
	objManager = objM;
	number = num;
}

SniperEnemy::~SniperEnemy()
{
}

void SniperEnemy::EnemyInit()
{
#pragma region �ϐ��̏�����
	//SetActive(false);
	HP = ECI::MAX_HP * SECI::MAX_HP;
	warningTime = ECI::WARNING_TIME * SECI::WARNING_TIME * 60;
	attackTime = ECI::ATTACK_TIME * SECI::ATTACK_TIME * 60;

	speed = ECI::MOVE_SPEED * SECI::MOVE_SPEED;
	radius = ECI::RADIUS * SECI::RADIUS;
	swingRange = ECI::SWING_RANGE * SECI::SWING_RANGE;
	barrelAngle = SECI::FAN_RANGE;
	turretAngle = 0.0f;
	attackLength = ECI::ATTACK_LENGTH * SECI::ATTACK_LENGTH;

	death = false;
	trackingPlayer = false;
	trackingBreadcrumb = false;
	RECEIVEREPORT_MODE = true;
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	DESTRUCT_MODE = ECI::DESTRUCT_MODE;
	//turnaroundMode = ECI::TURNAROUND_MODE;
	TURNAROUND_MODE = true;

	scale = SECI::SCALE;

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3().zero, radius));

	//�Z���T�[�̏�����
	fanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//�ʒu
	fanInfo.fanRange = 180.0f;									   //�Ƃ̊p�x
	fanInfo.length = 30.0f;										   //����
	fanInfo.rotate = fanRotateOrigin;							   //��]�p

	//�ŏ��͍��G���
	actionState = ActionState::SEARCH;

	//�}�b�v�̃N���A
	breadMap.clear();

	SetActive(false);

	testFloat = 0.0f;

#pragma endregion

#pragma region ���f���̓ǂݍ���

	//��Ԃ̖C�g(�㉺�Ɉړ����镔��)Barrel
	tankBarrel  = SECI::TANK_RLEG_NAME;
	tankBarrel2 = SECI::TANK_LREG_NAME;
	num = to_string(number);
	numBarrel  = tankBarrel  + num;
	numBarrel2 = tankBarrel2 + num;
	//modelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	//modelRender->AddModel(numBarrel, "Resouse/leg_kutu.obj", "Resouse/leg_sneaker.png");
	modelRender->AddModel(numBarrel,  "Resouse/leg_R.obj", "Resouse/leg_LR.png");
	modelRender->AddModel(numBarrel2, "Resouse/leg_L.obj", "Resouse/leg_LR.png");
	modelRender->SetAncPoint(numBarrel,  Vector3(0.0f, -2.0f, 0.0f));
	modelRender->SetAncPoint(numBarrel2, Vector3(0.0f, -2.0f, 0.0f));

	//��Ԃ̖C��(��̕���)Turret
	tankTurret = SECI::TANK_HEAD_NAME;
	num = to_string(number);
	numTurret = tankTurret + num;
	//modelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	//modelRender->AddModel(numTurret, "Resouse/head_hand.obj", "Resouse/face_color.png");
	modelRender->AddModel(numTurret, "Resouse/elf_head.obj", "Resouse/face_color.png");

	//��Ԃ̎ԑ�(���̕���)Body
	tankBody = SECI::TANK_BODY_NAME;
	num = to_string(number);
	numBody = tankBody + num;
	//modelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	//modelRender->AddModel(numBody, "Resouse/body_bow.obj", "Resouse/hand_bow_color.png");
	modelRender->AddModel(numBody, "Resouse/elf_body.obj", "Resouse/hand_bow_color.png");

#pragma endregion
}

void SniperEnemy::EnemyUpdate()
{
	Invincible(2);//���G����

	if (actionState == ActionState::WARNING)
	{
		if (testBool)
		{
			testFloat += value;
			if (testFloat > range)
			{
				testBool = false;
			}
		}
		else
		{
			testFloat -= value;
			if (testFloat < -range)
			{
				testBool = true;
			}
		}
	}
	else
	{
		testFloat = 0.0f;
	}



}

void SniperEnemy::EnemyRend()
{
	DirectXManager::GetInstance()->SetData3D();
	modelRender->Draw(numBarrel,  Vector3(position.x, position.y + 2.0f, position.z), Vector3(testFloat,  barrelAngle, 0), scale);//�r
	modelRender->Draw(numBarrel2, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-testFloat, barrelAngle, 0), scale);//�r
	modelRender->Draw(numTurret, position, Vector3(turretAngle, barrelAngle, 0), scale);//���Ǝ�
	modelRender->Draw(numBody, position, Vector3(0, barrelAngle, 0), scale);//�̂Ƌ|
}

void SniperEnemy::EnemyOnCollision(BaseCollider * col)
{
	/*if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}*/

	if (col->GetColObject()->GetType() == ObjectType::BREADCRUMB)
	{
		trackingBreadcrumb = false;
	}

	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//�_���[�W���󂯂�
		HP -= col->GetColObject()->GetDamage();

		/*��*/
		InitSearch();
	}
}

void SniperEnemy::EnemyImGuiDebug()
{
	//ImGui::SliderInt("HP", &destructCount, 0, 1000);
}

void SniperEnemy::Search()
{
	//ImGui::Text("ActionState == SEARCH");

	//��U����G�^
	SwingDirection(swingRange);
}

void SniperEnemy::Warning()
{
	//ImGui::Text("ActionState == WARNING");

	//�ǐՋ@�\
	TrackingObject();
}

void SniperEnemy::Attack()
{
	//ImGui::Text("ActionState == Attack");

	attackCount++;

	if (attackCount > attackTime)
	{
		attackCount = 0;
		Vector3 firePos = AngleToVectorY(fanInfo.rotate);

		//�e�𔭎ˁI�I
		objManager->Add(new NormalBullet(position + firePos, Vector3(0, -angle.y, 0), objManager, modelRender, effectManager, objType, bulletNumber));
		bulletNumber++;
		actionState = ActionState::WARNING;
	}
}

void SniperEnemy::Destruct()
{
	//ImGui::Text("ActionState == DESTRUCT");

	//DestructAction(objManager, modelRender);
}
