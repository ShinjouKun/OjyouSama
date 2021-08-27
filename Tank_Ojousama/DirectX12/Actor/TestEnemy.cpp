#include "TestEnemy.h"
#include "BreadCrumb.h"
#include "../Collision/SpherCollider.h"
#include "../Device/Input.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">�����ʒu</param>
/// <param name="angle">�����p�x</param>
/// <param name="objManager">�I�u�W�F�N�g�Ǘ���</param>
/// <param name="modelRender">���f���`��</param>
/// <param name="texRender">�|���S���`��</param>
/// <param name="num">���ʔԍ�</param>
TestEnemy::TestEnemy(
	Vector3 pos,
	Vector3 ang,
	ObjectManager * objM,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<TexRenderer> texRender,
	int num
)
	:modelRender(modelRender)/*, texRender(texRender)*/
{
	//�l�̑��
	position = pos;//�����Ӂ�BaseObject::position = pos;
	angle = ang;   //�����Ӂ�BaseObject::angle = ang;
	objManager = objM;
	number = num;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TestEnemy::~TestEnemy()
{
}

/// <summary>
/// �������ƃ��f���̓ǂݍ���
/// </summary>
void TestEnemy::Init()
{
#pragma region �ϐ��̏�����(�����Ȃ���)

	HP = 2;
	mapCount = 0;
	breadCount = 0;//�V�[�����̃p�������̐��B
	attackCount = 0;
	attackTime = 1 * 60;
	invincibleCount = 0;

	speed = 0.2f;
	radius = 1.0f;
	swingRange = 90.0f;
	barrelAngle = 180.0f;
	turretAngle = 0.0f;
	attackLength = 5.0f;

	death = false;
	isInvincible = false;
	hitSensor = false;
	swingSensor = false;
	trackingPlayer = false;
	trackingBreadcrumb = false;
	oneShot = false;

	angle = Vector3(0.0f, 180.0f, 0.0f);//�ԑ̂̌���
	lastBreadPos = Vector3(0, 0, 0);
	scale = Vector3(1.5f, 1.5f, 1.5f);

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3(position.x, position.y, position.z), radius));

	//�v���C���[�̈ʒu(��)
	searchPoint =
	{
		Vector3(100.0f,-3.0f,-50.0f),//�ʒu
		32.0f                        //���a
	};

	//�Z���T�[�̏�����
	fanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//�ʒu
	fanInfo.fanRange = 180.0f;									   //�Ƃ̊p�x
	fanInfo.length = 30.0f;										   //����
	fanInfo.rotate = fanRotateOrigin;							   //��]�p
	SetFanInfo(fanInfo.fanRange, fanInfo.length);//�ݒ�̓o�^�I

	actionState = ActionState::SEARCH;

	//�}�b�v�͍ŏ��ɏ��������Ƃ����ق��������炵���I
	breadMap.clear();

	//�U�����萶��
	area = new AttackArea(position, angle, objManager, modelRender, number);

#pragma endregion

#pragma region ���f���̓ǂݍ���

	//��Ԃ̖C�g(�㉺�Ɉړ����镔��)Barrel
	tankBarrel = "Barrel";
	num = to_string(number);
	numBarrel = tankBarrel + num;
	modelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	modelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//���S�_�̕ύX

	//��Ԃ̖C��(��̕���)Turret
	tankTurret = "Turret";
	num = to_string(number);
	numTurret = tankTurret + num;
	modelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	modelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//��Ԃ̎ԑ�(���̕���)Body
	tankBody = "Body";
	num = to_string(number);
	numBody = tankBody + num;
	modelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	modelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));

#pragma endregion

}

void TestEnemy::Update()
{
	//�����蔻��I�u�W�F�N�g����x��������
	if (!oneShot)
	{
		objManager->Add(area);
		oneShot = true;
	}

	Invincible(3);  //���G����
	SearchObject();      //�I�u�W�F�N�g����
	TrackingBreadcrumb();//�p��������ǂ�������
	ChangeState();		 //��ԕύX
	ImGuiDebug();		 //�f�o�b�O�\��

	//��������[
	mapCount = breadMap.size();
}

void TestEnemy::Rend()
{
	//���f���̕`��
	DirectXManager::GetInstance()->SetData3D();
	//tankM->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
	modelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	//tankM->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, -angle.y, 0), scale);
	modelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	modelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
}

void TestEnemy::OnCollison(const CollisonInfo & info)
{
	//�Փˏ���
	if (info.object->GetType() == ObjectType::BULLET ||
		info.object->GetType() == ObjectType::PLAYER)
	{
		//���G���ԂŖ�����΃_���[�W
		if (isInvincible) return;
		HP--;
		isInvincible = true;
	}

	if (info.object->GetType() == ObjectType::BREADCRUMB)
	{
		//trackingBreadcrumb = false;
	}
}

void TestEnemy::ImGuiDebug()
{
	//����̉���
	ImGui::SliderFloat("Range(Kakudo)", &fanInfo.fanRange, 0.0f, 300.0f);
	ImGui::SliderFloat("Length(Nagasa)", &fanInfo.length, 0.0f, 300.0f);
	ImGui::SliderFloat("Rotate(KaitenKakudo)", &fanInfo.rotate, -360, 360.0f);

	//�G���̉���
	float ePos[3] = { fanInfo.position.x,fanInfo.position.y,fanInfo.position.z };
	//float ePos[3] = { angle.x,angle.y,angle.z };
	ImGui::SliderFloat3("EnemyPosition", ePos, -500.0f, 500.0f);
	ImGui::SliderFloat("--Degree---", &angle.y, -360, 360);
	ImGui::SliderFloat("BarrelAngle", &barrelAngle, -360, 360);

	//��Ɠ_���������Ă���`�F�b�N������B
	ImGui::Checkbox("IsHitFan2Point", &hitSensor);
	ImGui::SliderInt("BreadNumber", &breadCount, 0, 500);
	ImGui::SliderInt("MapCount", &mapCount, 0, 500);

	ImGui::Checkbox("************************", &isInvincible);
}

void TestEnemy::SetFanInfo(float range, float length)
{
	fanInfo.fanRange = range;//�Ƃ̊p�x
	fanInfo.length = length;//����
}

void TestEnemy::Move(Vector3 otherPosition)
{
	Vector3 distance = otherPosition - position;
	//Vector3 len = position - otherPosition;
	float length = distance.Length();
	distance = distance.normal();//���K���Y�ꂸ��

	float radian = atan2(distance.x, distance.z);
	//��]�𔽉f
	angle.y = RadianToDegree(radian) + 180.0f;
	fanInfo.rotate = -angle.y - 90.0f;
	barrelAngle = fanInfo.rotate + 90.0f;
	//velocity = RotateY(angle.y + 90.0f) * speed;

	velocity = distance * speed;
	//�ʒu�ƃZ���T�[�𔽉f
	position += velocity;
	fanInfo.position = position;
}

void TestEnemy::ChangeState()
{
	//���S�����Ɉڂ�
	if (HP <= 0)
	{
		death = true;
		area->SetDeath(true);
	}

	//�U�����łȂ� & �v���C���[��ǂ��Ă��Ȃ� & �p��������H���Ă��Ȃ�
	if (!actionState == ActionState::ATTACK && !hitSensor && !trackingBreadcrumb)
	{
		actionState = ActionState::SEARCH;
	}

	switch (actionState)
	{
	case TestEnemy::SEARCH:
		Search();//��U��@�\
		break;
	case TestEnemy::WARNING:
		Warning();
		break;
	case TestEnemy::ATTACK:
		Attack();
		break;
	default:
		break;
	}
}

void TestEnemy::SearchObject()
{
	//�V�[����̃I�u�W�F�N�g��S�Č�������...
	for (auto& type : objManager->getUseList())
	{
		//�V�[����Ƀv���C���[�����݂��邱�Ƃ��m�F����
		if (type->GetType() == ObjectType::PLAYER)
		{
			SearchPlayer(type);
		}

		//�v���C���[�Ƃ͕ʂŁA�V�[����̃p�������̑��݂��m�F...
		if (type->GetType() == ObjectType::BREADCRUMB)
		{
			SearchBreadCrumb(type);
		}
	}
}

void TestEnemy::SearchPlayer(BaseObject * player)
{
	//�Z���T�[�̏����v���C���[���ōX�V
	searchPoint.position = player->GetPosition();
	searchPoint.radius = 1.0f;

	//�Z���T�[���X�V
	hitSensor = IsHitFanToPoint(fanInfo, searchPoint.position,searchPoint.radius);

	if (actionState == ActionState::ATTACK) return;

	//�����ŏ��߂āA�v���C���[�Ɠ������Ă�����̏����ɓ���B
	if (hitSensor)
	{
		trackingPlayer = true;
		trackingBreadcrumb = false;

		//�ǐՏ�ԂɈڍs
		actionState = ActionState::WARNING;

		//�ړ�
		Move(searchPoint.position);

		Vector3 distance = fanInfo.position - searchPoint.position;
		float length = distance.Length();

		//�ΏۂƂ̋�����1.0�ȉ��ɂȂ����瓞�������Ƃ���B
		if (length < attackLength)
		{
			actionState = ActionState::ATTACK;
		}
	}
	else
	{
		trackingPlayer = false;
	}
}

void TestEnemy::SearchBreadCrumb(BaseObject * breadcrumb)
{
	//�ʒu�擾 & �Z���T�[���X�V
	Vector3 breadPos = breadcrumb->GetPosition();
	searchPoint.position = breadPos;

	//�p��������H���Ă��� & �v���C���[��ǂ������Ă��鎞�͏������Ȃ�
	if (trackingBreadcrumb && trackingPlayer) return;

	//�T�[�`���C�g�ɓ������Ă���I�u�W�F�N�g�̂݁A�}�b�v�Ɋi�[
	if (IsHitFanToPoint(fanInfo, searchPoint.position))
	{
		//���݂���I�u�W�F�N�g�̌̔ԍ����擾
		//breadCount = breadcrumb->GetNumber();
		breadCount = breadcrumb->GetID();

		auto itr = breadMap.find(breadCount);
		//�}�b�v�Ɏw�肵���L�[�������Ă��Ȃ�������
		if (itr == breadMap.end())
		{
			breadMap.emplace(breadCount, breadPos);
		}

		////�utyr_emplace�v = �L�[�����݂��Ȃ��ꍇ�̂݁A�v�f���i�[����
		//breadMap.try_emplace(breadCount, type->GetPosition());
	}
}

void TestEnemy::TrackingBreadcrumb()
{
	//�v���C���[��ǐՒ��͏������Ȃ�
	if (trackingPlayer) return;

	//�ړ����Ă��Ȃ� && �}�b�v����łȂ�
	if (!trackingBreadcrumb && !breadMap.empty())
	{
		int lastKey = 0;
		for (auto itr = breadMap.begin(); itr != breadMap.end(); ++itr)
		{
			//�ŏ�����Ō�܂ŏ㏑�����邩��A���ʓI�ɍŌ�̗v�f��������H
			lastKey = itr->first;
		}

		////�ŏ��̗v�f���擾
		//auto begin = breadMap.begin();
		//lastKey = begin->first;

		//�Ō�̗v�f���擾
		lastBreadPos = breadMap[lastKey];

		if (!breadMap.empty())
		{
			//�}�b�v����U��ɂ���
			breadMap.clear();
		}

		//�ړ���Ԃɂ���
		trackingBreadcrumb = true;
	}

	if (trackingBreadcrumb)
	{
		//�ړ�
		Move(lastBreadPos);

		//�x����ԂɈڍs
		actionState = ActionState::WARNING;

		Vector3 distance = fanInfo.position - lastBreadPos;
		float length = distance.Length();

		//�ΏۂƂ̋�����1.0�ȉ��ɂȂ����瓞�������Ƃ���B
		if (length < 1.0f)
		{
			trackingBreadcrumb = false;
		}
	}
}

void TestEnemy::Search()
{
	//�ݒ�̏�����
	SetFanInfo();

	ImGui::Text("ActionState == SEARCH");

	//���݂̌����Ɛ�̌������X�V
	fanRotateOrigin = -angle.y - 90.0f;

	//���-270 �ő��-360 �ŏ���-180
	if (swingSensor)
	{
		fanInfo.rotate++;
		if (fanInfo.rotate > fanRotateOrigin + swingRange)//-179
		{
			swingSensor = false;
		}
	}
	else
	{
		fanInfo.rotate--;
		if (fanInfo.rotate < fanRotateOrigin - swingRange)//-361
		{
			swingSensor = true;
		}
	}

	//�C������]
	barrelAngle = fanInfo.rotate + 90.0f;
}

void TestEnemy::Warning()
{
	//���̎��͍��G�͈͂�傫���������B

	SetFanInfo(120.0f);

	ImGui::Text("ActionState == WARNING");


}

void TestEnemy::Attack()
{
	SetFanInfo(120.0f);
	ImGui::Text("ActionState == TRACKING");

	Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
	area->SetActive(true, position + areaPos, angle);

	attackCount++;

	if (attackCount > attackTime)
	{
		attackCount = 0;
		area->SetActive(false);
		actionState = ActionState::WARNING;
	}
}

void TestEnemy::Invincible(int time)
{
	if (!isInvincible) return;

	time *= 60;

	invincibleCount++;

	//���G���ԏI��
	if (invincibleCount > time)
	{
		invincibleCount = 0;
		isInvincible = false;
	}
}

void TestEnemy::ChangeDirection()
{
	//�ړ������������悤�ɂ����e�X�g�B

	Vector3 diff = position - previousPos;
	previousPos = position;
	diff.normalize();
	float radian = atan2(diff.x, diff.z);
	//��]�𔽉f
	angle.y = RadianToDegree(radian) + 180.0f;
}

float TestEnemy::DegreeToRadian(float angle)
{
	float radian;
	radian = angle * (PI / 180.0f);

	return radian;
}

float TestEnemy::RadianToDegree(float radian)
{
	float degree;
	degree = radian * (180.0f / PI);

	return degree;
}

Vector3 TestEnemy::AngleToVectorY(float angle)
{
	Vector3 vector;

	//�p�x�����W�A���p�ɖ߂�
	float radian = DegreeToRadian(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	vector = Vector3(x, 0, z);
	//�P�ʃx�N�g����
	vector.normalize();

	return vector;
}

float TestEnemy::VectorToAngleY(Vector3 vector)
{
	float angle = atan2f(vector.x, vector.z);

	RadianToDegree(angle);

	return angle;
}

bool TestEnemy::IsHitFanToPoint(FanInfomation fan, Vector3 point, float radius)
{
	//�@�@�_�Ɛ�S�_�̃x�N�g�������߂�
	Vector3 vectorFanToPoint =
	{
		point.x - fan.position.x,
		point.y - fan.position.y,
		point.z - fan.position.z,
	};

	//�A�@�@�̃x�N�g���̒��������߂�
	float vectorLength = sqrtf(
		(vectorFanToPoint.x * vectorFanToPoint.x) +
		(vectorFanToPoint.y * vectorFanToPoint.y) +
		(vectorFanToPoint.z * vectorFanToPoint.z)
	);

	//�B�@�x�N�g���̒����ƁA��͈̔͂̒������r
	if (fan.length + radius < vectorLength)
	{
		//��̕����������ꍇ�͂������Ă��Ȃ�
		return false;
	}



	//�C�@�~�ʂ̕����x�N�g�������߂�
	float rotateRadian = DegreeToRadian(fan.rotate);

	//�D�@0�x�̐�̊p�x�̒P�ʃx�N�g��(arc = �~��)
	Vector3 arcDirection = Vector3(1, 0, 0);

	//�E�@�x�N�g������]������(x,z)
	Vector3 rotateArcDirection
	{
		arcDirection.x * cosf(rotateRadian) + arcDirection.z * -sinf(rotateRadian),
		0.0f,
		arcDirection.x * 1.0f * sinf(rotateRadian) + arcDirection.z * cosf(rotateRadian)
	};

	//�F�@��Ɠ_�̃x�N�g����P�ʃx�N�g����
	Vector3 normalFanToPoint =
	{
		vectorFanToPoint.x / vectorLength,
		vectorFanToPoint.y / vectorLength,
		vectorFanToPoint.z / vectorLength,
	};

	//�G�@���ς̌v�Z
	float dot =
		normalFanToPoint.x * rotateArcDirection.x +
		normalFanToPoint.y * rotateArcDirection.y +
		normalFanToPoint.z * rotateArcDirection.z;

	//�H�@��͈̔͂�cos�̒l�ɂ���
	float fanCos = cosf(DegreeToRadian(fan.fanRange / 2.0f));

	//�I�@�_����͈͓̔��ɂ��邩��r������
	if (fanCos > dot)
	{
		//�������Ă��Ȃ�
		return false;
	}

	//�����܂œ��B�����瓖�����Ă���
	return true;
}
