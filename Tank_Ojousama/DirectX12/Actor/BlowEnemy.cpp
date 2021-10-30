#include "BlowEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../ConstInfomation/Enemy/BlowEnemyConstInfo.h"

namespace ECI = EnemyConstInfo;
namespace BECI = BlowEnemyConstInfo;

BlowEnemy::BlowEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objM,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<TexRenderer> texRender,
	shared_ptr<ParticleManager> effect,
	int num
) :modelRender(modelRender), effectManager(effect)
{
	position = pos;
	angle = ang;
	objManager = objM;
	number = num;
}

BlowEnemy::~BlowEnemy()
{
	attackArea->SetDeath(true);
}

void BlowEnemy::EnemyInit()
{
#pragma region �ϐ��̏�����

	HP = ECI::MAX_HP * BECI::MAX_HP;
	warningTime = ECI::WARNING_TIME * BECI::WARNING_TIME * 60;
	attackTime = ECI::ATTACK_TIME * BECI::ATTACK_TIME * 60;

	speed = ECI::MOVE_SPEED * BECI::MOVE_SPEED;
	radius = ECI::RADIUS * BECI::RADIUS;
	swingRange = ECI::SWING_RANGE * BECI::SWING_RANGE;
	barrelAngle = BECI::FAN_RANGE;
	turretAngle = 0.0f;
	attackLength = ECI::ATTACK_LENGTH * BECI::ATTACK_LENGTH;

	death = false;
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	destructMode = ECI::DESTRUCT_MODE;
	//turnaroundMode = ECI::TURNAROUND_MODE;
	turnaroundMode = true;

	scale = BECI::SCALE;

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3(position.x, position.y, position.z), radius));

	//�v���C���[�ʒu(��)������
	searchPoint.position = Vector3().zero;
	searchPoint.radius = radius;

	//�Z���T�[�̏�����----------------
	fanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//�ʒu
	fanInfo.fanRange = 180.0f;									   //�Ƃ̊p�x
	fanInfo.length = 30.0f;										   //����
	fanInfo.rotate = fanRotateOrigin;							   //��]�p
	//--------------------------------

	//�ŏ��͍��G���
	actionState = ActionState::SEARCH;

	//�}�b�v�̃N���A
	breadMap.clear();

	attackArea = new AttackArea(position, angle, objManager, modelRender, number);
	attackArea->SetActive(false);

	//����|�C���g�̏�����------------------------------------
	pointCount = sizeof(patrolPoint) / sizeof(patrolPoint[0]);
	patrolPoint[0] = Vector3(position.x, position.y, position.z);
	patrolPoint[1] = Vector3(position.x + 10, position.y, position.z);
	patrolPoint[2] = Vector3(position.x + 10, position.y, position.z + 10);
	patrolPoint[3] = Vector3(position.x, position.y, position.z + 10);
	//----------------------------------------------------------------

	currentTrigger = false;
	previousTri = false;
	onTrigger = false;

#pragma endregion

#pragma region ���f���̓ǂݍ���

	//��Ԃ̖C�g(�㉺�Ɉړ����镔��)Barrel
	tankBarrel = BECI::TANK_BARREL_NAME;
	num = to_string(number);
	numBarrel = tankBarrel + num;
	modelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	modelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//���S�_�̕ύX

	//��Ԃ̖C��(��̕���)Turret
	tankTurret = BECI::TANK_TURRET_NAME;
	num = to_string(number);
	numTurret = tankTurret + num;
	modelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	modelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//��Ԃ̎ԑ�(���̕���)Body
	tankBody = BECI::TANK_BODY_NAME;
	num = to_string(number);
	numBody = tankBody + num;
	modelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	modelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));

#pragma endregion
}

void BlowEnemy::EnemyUpdate()
{
	/*�����蔻��I�u�W�F�N�g�𐶐�*/
	CreateOneObject();

	///*�����@�\*/
	//DestructMode(BECI::MAX_HP / 2, destructMode);


	///*���G���� & �U���������*/
	////Invincible(ECI::REPORT_INTERVAL);//���G����
	Invincible(2);

	//int debugHP = GetID();
	//ImGui::SliderInt("MP-------------------", &debugHP, 0, 500);
}

void BlowEnemy::EnemyRend()
{
	//���f���̕`��
	DirectXManager::GetInstance()->SetData3D();
	modelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	modelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	modelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);

}

void BlowEnemy::EnemyOnCollision(BaseCollider * col)
{
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

void BlowEnemy::EnemyImGuiDebug()
{
#ifndef DEBUG

	//ImGui::SliderInt("HP", &HP, 0, 10);
	//ImGui::SliderInt("HP", &destructCount, 0, 1000);
	/*ImGui::Checkbox("Previous", &previousTri);*/

	//float ePos[3] = { fanInfo.position.x,fanInfo.position.y,fanInfo.position.z };
	//float ePos[3] = { angle.x,angle.y,angle.z };
	//ImGui::SliderFloat3("EnemyPosition", ePos, -500.0f, 500.0f);
	//ImGui::SliderFloat("HP", &barrelAngle, -1000.0f, 1000.0f);

#endif // DEBUG
}

void BlowEnemy::Search()
{
	//PatrolPoint(patrolPoint,pointCount);
	SwingDirection(swingRange);
}

void BlowEnemy::Warning()
{
	//ImGui::Text("ActionState == WARNING");

	attackArea->SetActive(false);

	TrackingObject();
}

void BlowEnemy::Attack()
{
	//ImGui::Text("ActionState == ATTACK");

	//Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
	//attackArea->SetActive(true, position + areaPos, -angle);

	attackCount++;

	//���b���Ƃɓ����蔻��������̑O���ɕ\������
	if (attackCount > attackTime)
	{
		attackCount = 0;
		attackArea->SetActive(false);

		//�U�����I�������x����Ԃɖ߂�
		actionState = ActionState::WARNING;
	}
}

void BlowEnemy::Destruct()
{
	//ImGui::Text("ActionState == DESTRUCT");

	DestructAction(modelRender);
}

void BlowEnemy::CreateOneObject()
{
	//�����蔻��I�u�W�F�N�g����x��������
	if (!onTrigger)
	{
		objManager->Add(attackArea);
		attackArea->SetActive(false);
		onTrigger = true;
	}
}
