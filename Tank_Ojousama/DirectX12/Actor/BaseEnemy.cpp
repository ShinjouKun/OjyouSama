#include "BaseEnemy.h"
#include "BreadCrumb.h"
#include "../Collision/SpherCollider.h"

BaseEnemy::~BaseEnemy()
{
}

void BaseEnemy::Init()
{
	EnemyInit();
}

void BaseEnemy::Update()
{
	EnemyUpdate();
}

void BaseEnemy::Rend()
{
	EnemyRend();
}

void BaseEnemy::OnCollison(BaseCollider * col)
{
	EnemyOnCollision(col);
}

void BaseEnemy::ImGuiDebug()
{
	EnemyImGuiDebug();
}

void BaseEnemy::Move(const Vector3 & otherPosition)
{
	//�����𒲂ׂ�(Vector3��float)
	Vector3 distance = otherPosition - position;
	float length = distance.Length();
	distance = distance.normal();//���K���Y�ꂸ��

	//��_�Ԃ̊p�x�����߂�
	float radian = atan2(distance.x, distance.z);
	//��]�𔽉f
	angle.y = Math::toDegrees(radian) + 180.0f;
	fanInfo.rotate = -angle.y - 90.0f;
	barrelAngle = fanInfo.rotate + 90.0f;

	velocity = distance * speed;
	//�ʒu�ƃZ���T�[�𔽉f
	position += velocity;
	fanInfo.position = position;
}

void BaseEnemy::SetFanInfo(float range, float length)
{
	//�p�x�ƒ������Đݒ�
	fanInfo.fanRange = range;//�Ƃ̊p�x
	fanInfo.length = length; //����
}

//��������ɌĂ΂�Ă��B
void BaseEnemy::ChangeState()
{
	//�I�u�W�F�N�g�̒ǐՊJ�n
	StartTracking();

	//���S�����Ɉڂ�
	if (HP <= 0)
	{
		death = true;
	}

	//�e��Ԃ��Ƃ̏���
	switch (actionState)
	{
	case BaseEnemy::SEARCH:
		Search();//��U��@�\
		break;
	case BaseEnemy::WARNING:
		Warning();
		break;
	case BaseEnemy::ATTACK:
		Attack();
		break;
	case BaseEnemy::DESTRUCT:
		Destruct();
		break;
	default:
		break;
	}
}

//memo : ������ɌĂ΂�Ă邩��Ȃ�ׂ��������y���������ˁB
void BaseEnemy::SearchObject(ObjectManager* objManager)
{
	//�v���C���[��ǂ��Ă��Ȃ� & �p�������ǐՋ@�\��ON�@�Ȃ珈�������s
	if (!trackingPlayer && breadcrumbMode)
	{
		//�V�[����̃I�u�W�F�N�g��S�Č�������...
		for (auto& type : objManager->getUseList())
		{
			//�V�[����̃p�������̑��݂��m�F
			if (type->GetType() == ObjectType::BREADCRUMB)
			{
				SearchBreadCrumb(type);
			}
		}
	}

	//�V�[����̃v���C���[�̑��݂��m�F
	SearchPlayer(objManager);
}

void BaseEnemy::SearchPlayer(ObjectManager* objManager)
{
	//�Z���T�[�̏����v���C���[���ōX�V
	searchPoint.radius = 1.0f;
	//searchPoint.position = objManager->GetPlayer().GetPosition();

	//�Z���T�[���X�V
	hitSensor = IsHitFanToPoint(fanInfo, searchPoint.position, searchPoint.radius);

	//�v���C���[���Z���T�[�̒��ɓ����Ă�����
	if (hitSensor)
	{
		trackingPlayer = true;     //�v���C���[�̒ǐՂ��J�n����
		trackingBreadcrumb = false;//�p�������̒ǐՂ��I������

		//�}�b�v����U��ɂ���
		if (!breadMap.empty())
		{
			breadMap.clear();
		}
	}
	else trackingPlayer = false;
}

void BaseEnemy::SearchBreadCrumb(BaseObject * breadcrumb)
{
	//�p��������H���Ă��� & �v���C���[��ǂ��Ă��� �Ȃ珈�����Ȃ�
	if (trackingBreadcrumb && trackingPlayer) return;

	//�ʒu�擾 & �Z���T�[���X�V
	Vector3 breadPos = breadcrumb->GetPosition();
	searchPoint.position = breadPos;

	//�T�[�`���C�g�ɓ������Ă���I�u�W�F�N�g�̂݁A�}�b�v�Ɋi�[
	if (IsHitFanToPoint(fanInfo, searchPoint.position))
	{
		//�I�u�W�F�N�g�̌̔ԍ����擾
		breadCount = static_cast<int>(breadcrumb->GetID());

		//�}�b�v�Ɏw�肵���L�[�������Ă��Ȃ�������ǉ�����
		auto itr = breadMap.find(breadCount);
		if (itr == breadMap.end())
		{
			breadMap.emplace(breadCount, breadPos);
		}
	}
}

void BaseEnemy::StartTracking()
{
	//���G�����炵���U������Ȃ��悤�ɂ���
	if (actionState == ActionState::SEARCH)
	{
		if (trackingPlayer || trackingBreadcrumb)
		{
			actionState = ActionState::WARNING;
		}
	}
}

void BaseEnemy::TrackingBreadcrumb()
{
	//�v���C���[��ǐՒ� �Ȃ珈�����Ȃ�
	if (trackingPlayer) return;

	//�p��������ǐՒ��łȂ� && �}�b�v����łȂ�
	if (!trackingBreadcrumb && !breadMap.empty())
	{
		int lastKey = 0;
		for (auto itr = breadMap.begin(); itr != breadMap.end(); ++itr)
		{
			//�ŏ�����Ō�܂ŏ㏑�����邩��A���ʓI�ɍŌ�̗v�f��������H
			lastKey = itr->first;
		}

		//�Ō�̗v�f���擾
		lastBreadPos = breadMap[lastKey];

		//�}�b�v����U��ɂ���
		if (!breadMap.empty())
		{
			breadMap.clear();
		}

		//�ړ���Ԃɂ���
		trackingBreadcrumb = true;
	}
}

void BaseEnemy::Damage(int damage, ObjectManager* objManager)
{
	//���G���Ԓ� �Ȃ珈�����Ȃ�
	if (isInvincible) return;

	HP -= damage;
	isInvincible = true;

	//
	DicideTurnAround(objManager);

	//���S�����Ɉڂ�
	if (HP <= 0)
	{
		death = true;
	}
}

void BaseEnemy::Invincible(int time)
{
	//���G���Ԓ��łȂ� �Ȃ珈�������Ȃ�
	if (!isInvincible) return;

	time *= 60;

	invincibleCount++;

	//�U������J�n
	TurnAround(time);

	//���G���ԏI��
	if (invincibleCount > time)
	{
		invincibleCount = 0;
		isInvincible = false;
	}
}

void BaseEnemy::SwingDirection(float range)
{
	//���݂̌������ŐV�ɍX�V
	fanRotateOrigin = -angle.y - 90.0f;

	//���-270 �ő��-360 �ŏ���-180
	if (swingSensor)//����]
	{
		fanInfo.rotate++;
		if (fanInfo.rotate > fanRotateOrigin + swingRange)
		{
			swingSensor = false;
		}
	}
	else//�E��]
	{
		fanInfo.rotate--;
		if (fanInfo.rotate < fanRotateOrigin - swingRange)
		{
			swingSensor = true;
		}
	}

	//�C������]
	barrelAngle = fanInfo.rotate + 90.0f;
}

void BaseEnemy::PatrolPoint(const Vector3 points[], int pointCount)
{
	//�z��̔ԍ��̈ʒu���擾
	Vector3 targetPosition = points[currentPointNumber];

	//�ړ�
	Move(targetPosition);

	//���������Ƃ���
	if (InsideDistance(targetPosition, 0.1f))
	{
		//�S�[���Ƀg��������(�E�ӂ́A�z��̗v�f�������߂Ă��܂�)
		if (currentPointNumber + 1 == pointCount)
		{
			currentPointNumber = 0;
		}
		else
		{
			currentPointNumber++;
		}
	}
}

void BaseEnemy::TrackingObject()
{
	//�v���C���[��ǂ��Ă���Ƃ�
	if (trackingPlayer)
	{
		//�ړ�
		Move(searchPoint.position);

		//�ΏۂƂ̋������ȉ��ɂȂ����瓞�������Ƃ���B
		if (InsideDistance(searchPoint.position, attackLength))
		{
			actionState = ActionState::ATTACK;
		}
	}
	else
	{
		// �p��������ǂ�������
		TrackingBreadcrumb();

		if (trackingBreadcrumb)
		{
			//�ړ�
			Move(lastBreadPos);

			//�ΏۂƂ̋������ȉ��ɂȂ����瓞�������Ƃ���B
			if (InsideDistance(lastBreadPos, 1.0f))
			{
				trackingBreadcrumb = false;
			}
		}
	}

	//��莞�Ԃō��G��Ԃɖ߂�
	if (!trackingPlayer && !trackingBreadcrumb)
	{
		warningCount++;

		if (warningCount > warningTime)
		{
			warningCount = 0;
			actionState = ActionState::SEARCH;
		}
	}
}

void BaseEnemy::DestructAction(ObjectManager * objManager, shared_ptr<ModelRenderer> modelRender)
{
	/*�����J�E���g�𑝂₷*/
	destructCount++;

	//��莞�ԃv���C���[��ǂ���������A�傫�߂̓����蔻����o���Ď��ʁB
	if (destructCount > 2 * 60)
	{
		Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
		destructArea->SetActive(true, position, -angle, Vector3(5, 5, 5));
		destructArea->SetDestroy(true, 2);
		HP = 0;
	}

	//�ړ�
	Move(searchPoint.position);

	//��x�������s
	if (!isDestruct)
	{
		isDestruct = true;
		speed *= 2.0f;

		destructArea = new AttackArea(position, angle, objManager, modelRender, number);
		objManager->Add(destructArea);
	}
}

void BaseEnemy::DicideTurnAround(ObjectManager* objManager)
{
	//���G��ԂłȂ� or �U������@�\��OFF �Ȃ珈�������Ȃ�
	if (actionState != ActionState::SEARCH || !turnaroundMode) return;

	//���������ʒu�ƁA���݂̎����̌������ꎞ�ۑ�����B
	//hitPos = objManager->GetPlayer().GetPosition();
	hitAngle = angle;
	actionState = ActionState::WARNING;
}

void BaseEnemy::TurnAround(int time)
{
	//�U������@�\��OFF or �v���C���[��ǂ��Ă��� or ������� �Ȃ珈�������Ȃ�
	if (!turnaroundMode || trackingPlayer || isDestruct) return;

	//�@�����𒲂ׂ�
	Vector3 distance = hitPos - position;
	//�A���K���Y�ꂸ��
	distance = distance.normal();
	//�B��_�Ԃ̊p�x�����߂�
	float radian = atan2(distance.x, distance.z);
	//�C��]������p�x = ���������ꏊ�Ƃ̊p�x - ���݂̎����̊p�x
	float rotValue = (Math::toDegrees(radian) + 180) - hitAngle.y;
	//�D��]���������b���Ŋ���
	float rotSecond = rotValue / time;
	//�E���ݎ��Ԃ�������
	float rotateTime = rotSecond * invincibleCount;
	//�F���� = ���݂̎����̊p�x + ��]������p�x
	angle.y = hitAngle.y + rotateTime;
	fanInfo.rotate = (-angle.y - 90.0f);
	barrelAngle = (fanInfo.rotate + 90.0f);
}

void BaseEnemy::DestructMode(int hpLine, bool destructMode)
{
	//�������[�h��OFF �Ȃ珈�����Ȃ��B
	if (!destructMode) return;

	//�̗͂����ȉ��ɂȂ����玩�����[�h�ɂȂ�B
	if (HP <= hpLine)
	{
		actionState = ActionState::DESTRUCT;
	}
}

void BaseEnemy::ChangeDirection()
{
}

Vector3 BaseEnemy::AngleToVectorY(float angle)const
{
	Vector3 vector = Vector3(0, 0, 0);

	//�p�x�����W�A���p�ɖ߂�
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}

float BaseEnemy::VectorToAngleY(const Vector3 & vector) const
{
	float angle = atan2f(vector.x, vector.z);

	angle = Math::toDegrees(angle);

	return angle;
}

bool BaseEnemy::InsideDistance(const Vector3 & otherPos, float dist)
{
	Vector3 distance = otherPos - position;
	float length = distance.Length();

	//�͈͊O�������炱���ŏI���
	if (length > dist)
	{
		return false;
	}

	return true;
}

bool BaseEnemy::IsHitFanToPoint(const FanInfomation & fan, const Vector3 & point, float radius) const
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
	float rotateRadian = Math::toRadians(fan.rotate);

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
	float fanCos = cosf(Math::toRadians(fan.fanRange / 2.0f));

	//�I�@�_����͈͓̔��ɂ��邩��r������
	if (fanCos > dot)
	{
		//�������Ă��Ȃ�
		return false;
	}

	//�����܂œ��B�����瓖�����Ă���
	return true;
}
