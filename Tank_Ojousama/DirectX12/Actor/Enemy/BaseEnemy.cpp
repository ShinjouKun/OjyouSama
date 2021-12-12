#include "BaseEnemy.h"
#include "../../Collision/SpherCollider.h"

#include "ReportArea.h"
#include "../TestWayPoint.h"
#include "../WayPointManager.h"
#include "../BreadCrumbCreater.h"
#include "../TestBreadCrumb.h"

EnemyAI* BaseEnemy::mEnemyAI = nullptr;
ObjectManager* BaseEnemy::mManager = nullptr;
std::shared_ptr<ModelRenderer> BaseEnemy::mRend = nullptr;
std::shared_ptr<ParticleManager> BaseEnemy::mPart = nullptr;
std::shared_ptr<BreadCrumbCreater> BaseEnemy::mBreadCreator = nullptr;

Vector3 BaseEnemy::mAttackTarget = Vector3(0, 0, 0);

BaseEnemy::~BaseEnemy()
{

}

void BaseEnemy::Init()
{
	//�ŏ��͕\�����
	SetActive(false);

	Initialize();
	EnemyInit();
}

void BaseEnemy::Update()
{
	//1�t���[���O�̈ʒu��ۑ����Ă����B
	mPreviousPosition = position - velocity;

	mPlayerPosition = mManager->GetPlayer().GetPosition();

	////������Ԃ̊Ď�
	//AliveSurveillance();

	//if (mAdvanceFlag)
	/*{
		int t = static_cast<int>(mMoveState);

		ImGui::SliderInt("MoveState", &t, 0, 20);
	}*/



	//�J�����̓����蔻�莟��ŁA���̏����͏����B
	//if (!GetActive()) return;

	//if (RECEIVEREPORT_MODE && !trackingPlayer && !trackingBreadcrumb)
	//{
	//	//5:AI����񍐎�ɋ߂��G��ID���X�g���󂯎��
	//	for (int i = 0, end = static_cast<int>(mEnemyAI->GetIDList().size()); i < end; i++)
	//	{
	//		//6:���g�Ǝ󂯎����ID����v���Ă��邩���m�F����
	//		if (GetID() == mEnemyAI->GetIDList()[i])
	//		{
	//			if (!InsideDistance(mEnemyAI->GetPositionList(), 5.0f))
	//			{
	//				//7:��v���Ă�����A�񍐎�̈ʒu�Ɉړ�����
	//				Move(mEnemyAI->GetPositionList());
	//			}
	//		}
	//	}
	//}

	EnemyUpdate();
}

void BaseEnemy::Rend()
{
	//if (!GetActive()) return;

	EnemyRend();
}

//�����̓I�u�W�F�N�g�ɂԂ���������������Ȃ��B
void BaseEnemy::OnCollison(BaseCollider * col)
{
	//if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	//{
	//	//�J�����ɓ������Ă���Ƃ��A�`����s���B
	//	SetActive(true);
	//}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}

	if (col->GetColObject()->GetType() == ObjectType::ENEMY)
	{
		////�����̔ԍ��������菬����������
		//if (col->GetColObject()->GetID() > GetID())
		//{
		//	position = mPreviousPosition;
		//}
	}

	//if (!GetActive()) return;

	EnemyOnCollision(col);
}

void BaseEnemy::ImGuiDebug()
{
	EnemyImGuiDebug();
}

void BaseEnemy::Initialize()
{
	//number = 0;
	attackCount = 0;

	mTrackingPlayer = false;
	mTrackingBreadcrumb = false;
	mTrackingAttackArea = false;
	mTrackingTurret = false;
	mHitBorderLine = false;

	warningCount = 0;
	invincibleCount = 0;
	destructCount = 0;
	breadCount = 0;
	patrolCount = 0;
	searchCount = 0;
	loopCount = 0;
	moveCount = 0;
	arrayCount = 0;
	damage = 5;

	isInvincible = false;
	mHitSensor = false;
	swingSensor = false;
	isDestruct = false;
	finishSearchWay = false;
	hitReportArea = false;
	moveWayPoint = false;
	goalFlag = false;
	isActive = true;

	lastBreadPos = Vector3().zero;
	previousPos = Vector3().zero;
	hitPos = Vector3().zero;
	hitAngle = Vector3().zero;
	goalPoint = Vector3().zero;
	resultPoint = Vector3().zero;

	breadMap.clear();
	moveList.clear();

	mSmokeTimer = std::make_shared<Timer>();
	mSmokeTimer->setTime(1.0f);

	//mTarget.clear();
	//mPointList.clear();
	//mPointList.shrink_to_fit();//�������̍폜

	//mReportArea = nullptr;
}

void BaseEnemy::AliveSurveillance()
{
	//������Ԃ������玀�S������
	if (mDeathFlag)
	{
		death = true;
	}

	if (HP <= 0)
	{
		//�̗͂�0�ȉ��Ȃ牼����Ԃɂ���
		mDeathFlag = true;
	}
}

void BaseEnemy::MovePoint(const Vector3 & otherPosition)
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
	mFireAngle = fanInfo.rotate + 90.0f;

	velocity = distance * speed;
	//�ʒu�ƃZ���T�[�𔽉f
	position += velocity;
	fanInfo.position = position;
}

void BaseEnemy::MovePointY(const Vector3 & otherPosition)
{
	//Y���Ȃ����ʒu����A���������߂�
	Vector3 holdTarget = Vector3(otherPosition.x, 0.0f, otherPosition.z);
	Vector3 holdPosition = Vector3(position.x, 0.0f, position.z);
	Vector3 distance = holdTarget - holdPosition;
	float length = distance.Length();
	distance = distance.normal();

	//Y���Ȃ�������
	Vector3 distHold = Vector3(distance.x, 0.0f, distance.z);

	//��_�Ԃ̊p�x�����߂�
	float radian = atan2(distHold.x, distHold.z);
	//��]�𔽉f
	angle.y = Math::toDegrees(radian) + 180.0f;
	fanInfo.rotate = -angle.y - 90.0f;
	mFireAngle = fanInfo.rotate + 90.0f;

	//�e���A�����ʒu��艺�ɍs���Ȃ���I
	velocity = distHold * speed;
	position += velocity;
	fanInfo.position = position;
}

void BaseEnemy::MoveDirection(const Vector3 & direction)
{
	//�����𒲂ׂ�(Vector3��float)
	Vector3 distance = direction;
	distance = distance.normal();//���K���Y�ꂸ��

	//��_�Ԃ̊p�x�����߂�
	float radian = atan2(distance.x, distance.z);
	//��]�𔽉f
	angle.y = Math::toDegrees(radian) + 180.0f;
	fanInfo.rotate = -angle.y - 90.0f;
	mFireAngle = fanInfo.rotate + 90.0f;

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

	//�Ӗڏ��
	SmokeBlind();
}

void BaseEnemy::SearchObject()
{
	if (mHitSmokeFlag) return;

	//�U�����_������
	SearchAttackTarget();

	//�V�[����̃v���C���[�̑��݂��m�F
	SearchPlayer();

	//�v���C���[��ǂ��Ă��Ȃ� & �p�������ǐՋ@�\��ON�@�Ȃ珈�������s
	if (!mTrackingPlayer && breadcrumbMode)
	{
		auto BList = mBreadCreator->GetBreadList();

		for (int i = 0, end = static_cast<int>(BList.size()); i < end; i++)
		{
			SearchBreadCrumb(*BList[i]);
		}
	}
}

void BaseEnemy::SearchPlayer()
{
	//�U�����_��_���Ă���Ƃ��͏������Ȃ�
	if (mMoveState == MoveState::CHASE_ATTACKTARGET) return;

	//�Z���T�[���X�V
	mHitSensor = IsHitFanToPoint(fanInfo, mPlayerPosition, 1.0f);

	//�v���C���[���Z���T�[�̒��ɓ����Ă�����
	if (mHitSensor)
	{
		mTrackingPlayer = true;     //�v���C���[�̒ǐՂ��J�n����
		mTrackingBreadcrumb = false;//�p�������̒ǐՂ��I������

		//�v���C���[�ǐՏ��
		mMoveState = MoveState::CHASE_PLAYER;

		//�}�b�v����U��ɂ���
		if (!breadMap.empty())
		{
			breadMap.clear();
		}
	}
	else mTrackingPlayer = false;
}

void BaseEnemy::SearchAttackTarget()
{
	//���_�i�s���[�h�łȂ� �Ȃ珈�����Ȃ�
	if (!mAdvanceFlag) return;

	//�Z���T�[���X�V
	mHitSensor = IsHitFanToPoint(fanInfo, mAttackTarget, 1.0f);

	//�U���Ώۂ��Z���T�[�ɐG��Ă�����
	if (mHitSensor)
	{
		mTrackingPlayer = false;    //�v���C���[�̒ǐՂ��I������
		mTrackingBreadcrumb = false;//�p�������̒ǐՂ��I������
		mTrackingAttackArea = true; //���_�i�U���J�n

		//�U�����_�ǐՏ��
		mMoveState = MoveState::CHASE_ATTACKTARGET;

		//�}�b�v����U��ɂ���
		if (!breadMap.empty())
		{
			breadMap.clear();
		}
	}
	else mTrackingAttackArea = false;
}

void BaseEnemy::SearchTurret()
{
	//�Z���T�[���X�V
	mHitSensor = IsHitFanToPoint(fanInfo, mAttackTarget, 1.0f);

	//�U���Ώۂ��Z���T�[�ɐG��Ă�����
	if (mHitSensor)
	{
		mTrackingPlayer = false;    //�v���C���[�̒ǐՂ��I������
		mTrackingBreadcrumb = false;//�p�������̒ǐՂ��I������
		mTrackingAttackArea = false; //���_�i�U���J�n
		mTrackingTurret = true;

		//�U�����_�ǐՏ��
		mMoveState = MoveState::CHASE_TURRET;

		//�}�b�v����U��ɂ���
		if (!breadMap.empty())
		{
			breadMap.clear();
		}
	}
	else mTrackingTurret = false;
}

void BaseEnemy::SearchBreadCrumb(const TestBreadCrumb & breadCrumb)
{
	//�p��������H���Ă��� & �v���C���[��ǂ��Ă��� �Ȃ珈�����Ȃ�
	if (mTrackingBreadcrumb && mTrackingPlayer) return;

	//�U�����_��_���Ă���Ƃ��͏������Ȃ�
	if (mMoveState == MoveState::CHASE_ATTACKTARGET) return;

	//�ʒu�擾 & �Z���T�[���X�V
	Vector3 breadPos = breadCrumb.GetPosition();

	//�Z���T�[���X�V
	mHitSensor = IsHitFanToPoint(fanInfo, breadPos);

	//�T�[�`���C�g�ɓ������Ă���I�u�W�F�N�g�̂݁A�}�b�v�Ɋi�[
	if (mHitSensor)
	{
		//�p�������ǐՏ��
		mMoveState = MoveState::CHASE_BREADCRUMB;

		//�I�u�W�F�N�g�̌̔ԍ����擾
		breadCount = breadCrumb.GetBreadNumber();

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
	if (mMoveState == MoveState::SEARCH)
	{
		if (mTrackingPlayer || mTrackingBreadcrumb || mTrackingAttackArea)
		{
			mMoveState = MoveState::WARNING;
		}
	}
}

void BaseEnemy::TrackingBreadcrumb()
{
	//�v���C���[��ǐՒ� �Ȃ珈�����Ȃ�
	if (mTrackingPlayer) return;

	//�p��������ǐՒ��łȂ� && �}�b�v����łȂ�
	if (!mTrackingBreadcrumb && !breadMap.empty())
	{
		int lastKey = 0;
		for (auto itr = breadMap.begin(), end = breadMap.end(); itr != end; ++itr)
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
		mTrackingBreadcrumb = true;
	}
}

void BaseEnemy::Invincible(int time)
{
	//���G���Ԓ��łȂ� �Ȃ珈�������Ȃ�
	if (!isInvincible) return;

	time *= 60;

	invincibleCount++;

	mMoveState = MoveState::TURN_AROUND;

	//�U������J�n
	TurnAround(time);

	//���G���ԏI��
	if (invincibleCount > time)
	{
		invincibleCount = 0;
		isInvincible = false;
	}
}

void BaseEnemy::SmokeBlind()
{
	//�X���[�N�ɐG��Ă��Ȃ���Ώ������Ȃ�
	if (!mHitSmokeFlag) return;

	//ImGui::Checkbox("----------------------------", &mHitSmokeFlag);

	mSmokeTimer->update();

	if (mSmokeTimer->isTime())
	{
		mSmokeTimer->setTime(1.0f);
		mHitSmokeFlag = false;
	}
}

void BaseEnemy::SwingDirection(float range)
{
	//���݂̌������ŐV�ɍX�V
	mFanRotateOrigin = -angle.y - 90.0f;

	//���-270 �ő��-360 �ŏ���-180
	if (swingSensor)//����]
	{
		fanInfo.rotate++;
		if (fanInfo.rotate > mFanRotateOrigin + mSwingRange)
		{
			swingSensor = false;
		}
	}
	else//�E��]
	{
		fanInfo.rotate--;
		if (fanInfo.rotate < mFanRotateOrigin - mSwingRange)
		{
			swingSensor = true;
		}
	}

	//�C������]
	mFireAngle = fanInfo.rotate + 90.0f;
}

void BaseEnemy::PatrolPoint(const Vector3 points[], int pointCount)
{
	//�z��̔ԍ��̈ʒu���擾
	Vector3 targetPosition = points[patrolCount];

	//�ړ�
	MovePoint(targetPosition);

	//���������Ƃ���
	if (InsideDistance(targetPosition, 0.1f))
	{
		//�S�[���Ƀg��������(�E�ӂ́A�z��̗v�f�������߂Ă��܂�)
		if (patrolCount + 1 == pointCount)
		{
			patrolCount = 0;
		}
		else
		{
			patrolCount++;
		}
	}
}

void BaseEnemy::TrackingObject()
{
	if (mHitSmokeFlag) return;

	switch (mMoveState)
	{
	case BaseEnemy::NOT_FIND:

		moveFlag = false;

		if (mAdvanceFlag)
		{
			mMoveState = MoveState::ADVANCE_BORDWERLINE;
		}
		else
		{
			if (mTrackingPlayer)
			{
				mMoveState = MoveState::CHASE_PLAYER;
			}
			else if (mTrackingBreadcrumb)
			{
				mMoveState = MoveState::CHASE_BREADCRUMB;
			}
		}

		break;
	case BaseEnemy::CHASE_PLAYER:

		moveFlag = true;

		//���_�i�U���[�h�̎� &�͈͓��ɍU�����_������Ƃ�
		if (mAdvanceFlag && mTrackingAttackArea)
		{
			//�U�����_��_���悤�ɂ���
			mMoveState = MoveState::CHASE_ATTACKTARGET;
		}
		else//���_�i�U���[�h�łȂ� & �͈͓��ɋ��_���Ȃ�
		{
			//�ړ�
			MovePoint(mPlayerPosition);

			//�ΏۂƂ̋������ȉ��ɂȂ����瓞�������Ƃ���B
			if (InsideDistance(mPlayerPosition, mAttackLength))
			{
				mAttackFlag = true;
				moveFlag = false;
			}
		}

		break;
	case BaseEnemy::CHASE_BREADCRUMB:

		moveFlag = true;

		//���_�i�U���[�h�̎� &�͈͓��ɍU�����_������Ƃ�
		if (mAdvanceFlag && mTrackingAttackArea)
		{
			//�U�����_��_���悤�ɂ���
			mMoveState = MoveState::CHASE_ATTACKTARGET;
		}
		else
		{
			if (mTrackingPlayer)
			{
				//�v���C���[��_���悤�ɂ���
				mMoveState = MoveState::CHASE_PLAYER;
			}
		}

		// �p��������ǂ�������
		TrackingBreadcrumb();

		if (mTrackingBreadcrumb)
		{
			//�ړ�
			MovePoint(lastBreadPos);

			//�ΏۂƂ̋������ȉ��ɂȂ����瓞�������Ƃ���B
			if (InsideDistance(lastBreadPos, 1.0f))
			{
				mTrackingBreadcrumb = false;
			}
		}

		//��莞�Ԃō��G��Ԃɖ߂�
		warningCount++;

		if (warningCount > warningTime)
		{
			warningCount = 0;

			if (mAdvanceFlag)
			{
				if (mHitBorderLine)
				{
					mMoveState = MoveState::ADVANCE_BORDWERLINE;
				}
				else
				{
					mMoveState = MoveState::ADVANCE_ATTACKTARGET;
				}
			}
			else
			{
				mMoveState = MoveState::NOT_FIND;
			}
		}

		break;
	case BaseEnemy::CHASE_ATTACKTARGET:

		moveFlag = true;

		//�ړ�
		MovePoint(mAttackTarget);

		//�ΏۂƂ̋������ȉ��ɂȂ����瓞�������Ƃ���B
		if (InsideDistance(mAttackTarget, mAttackLength))
		{
			mAttackFlag = true;
			moveFlag = false;
		}

		break;

	case BaseEnemy::CHASE_TURRET:

		moveFlag = true;

		/*�^���b�g�̈ʒu�̎擾���@���l����
		  �{�X�̍s����HP�ŊǗ����A�U���p�^�[����Ԃ�
		  */

		  //�ړ�
		MovePoint(mAttackTarget);

		//�ΏۂƂ̋������ȉ��ɂȂ����瓞�������Ƃ���B
		if (InsideDistance(mAttackTarget, mAttackLength))
		{
			mAttackFlag = true;
			moveFlag = false;
		}

		break;

	case BaseEnemy::ADVANCE_BORDWERLINE:

		moveFlag = true;

		//�{�[�_�[���C���ɐG��Ă��邩
		if (mHitBorderLine)
		{
			//�U�����_�Ɍ������Ĉړ�
			mMoveState = MoveState::ADVANCE_ATTACKTARGET;
		}
		else
		{
			//�G���܂ł͂܂������ړ�
			MoveDirection(mAdvanceDirection);
		}

		//�v���C���[�ɐG�ꂽ��
		if (mTrackingPlayer)
		{
			mMoveState = MoveState::CHASE_PLAYER;
		}

		break;
	case BaseEnemy::ADVANCE_ATTACKTARGET:

		moveFlag = true;

		if (mAdvanceFlag && mTrackingAttackArea)
		{
			mMoveState = MoveState::CHASE_ATTACKTARGET;
		}
		else if (mTrackingPlayer)
		{
			mMoveState = MoveState::CHASE_PLAYER;
		}
		else
		{
			MovePoint(mAttackTarget);
		}

		break;

	case BaseEnemy::TURN_AROUND:

		//��莞�Ԃō��G��Ԃɖ߂�
		warningCount++;

		if (warningCount > warningTime)
		{
			warningCount = 0;

			if (mAdvanceFlag)
			{
				if (mHitBorderLine)
				{
					mMoveState = MoveState::ADVANCE_BORDWERLINE;
				}
				else
				{
					mMoveState = MoveState::ADVANCE_ATTACKTARGET;
				}
			}
			else
			{
				mMoveState = MoveState::NOT_FIND;
			}
		}

		break;
		default:
			break;
	}

	////��莞�Ԃō��G��Ԃɖ߂�
	//if (!mTrackingPlayer && !mTrackingBreadcrumb)
	//{
	//	warningCount++;

	//	if (warningCount > warningTime)
	//	{
	//		warningCount = 0;
	//		mMoveState = MoveState::SEARCH;
	//	}
	//}
}

void BaseEnemy::DestructAction(shared_ptr<ModelRenderer> modelRender)
{
	///*�����J�E���g�𑝂₷*/
	//destructCount++;

	////��莞�ԃv���C���[��ǂ���������A�傫�߂̓����蔻����o���Ď��ʁB
	//if (destructCount > 2 * 60)
	//{
	//	Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
	//	destructArea->SetActive(true, position, -angle, Vector3(5, 5, 5));
	//	destructArea->SetDestroy(true, 2);
	//	HP = 0;
	//}

	////�ړ�
	//Move(mPlayerPosition);

	////��x�������s
	//if (!isDestruct)
	//{
	//	isDestruct = true;
	//	speed *= 2.0f;

	//	destructArea = new AttackArea(position, angle, mManager, modelRender, number);
	//	mManager->Add(destructArea);
	//}
}

void BaseEnemy::InitSearch()
{
	//���łɕ񍐂��Ă��� or ���݈ړ��� �Ȃ�\�����Ȃ�
	if (isInvincible || moveWayPoint || mTrackingPlayer || mTrackingAttackArea) return;

	isInvincible = true;

	//�U������@�\�ǉ�
	DicideTurnAround();

	//���S�����Ɉڂ�
	if (HP <= 0)
	{
		mDeathFlag = true;
		return;
	}

	////2:�����̈ʒu����AI�ɓ]��
	//mEnemyAI->SetValue(position);
}

//void BaseEnemy::GetAllWayPoint()
//{
//	////�S�Ẵ|�C���g�̃t���O��������
//	//mManager->ResetFlag();
//
//	//mPointList.clear();
//	//mPointList.resize(mManager->GetTestPointList().size());
//	//mPointList = mManager->GetTestPointList();
//
//	////�ړ����X�g�̃��������m��
//	//moveList.reserve(mPointList.size());
//
//	//mManager->Remove();
//	//mWay.reset();//����肭�����Ă邩�킩��Ȃ�
//}
//
//shared_ptr<TestWayPoint> BaseEnemy::NearWayPointStartTest(const Vector3 & point) const
//{
//	float distance = 0;
//	float mostDistance = 0;
//	shared_ptr<TestWayPoint> wayPoint;
//
//	for (int i = 0, end = static_cast<int>(mPointList.size()); i < end; i++)
//	{
//		Vector3 dist = mPointList[i]->GetPosition() - point;
//		distance = dist.Length();
//
//		//�܂��g�p���Ă��Ȃ����̂̂ݏ������s��
//		if (!mPointList[i]->GetUseFlag())
//		{
//			if (mostDistance == 0 || mostDistance > distance)
//			{
//				mostDistance = distance;
//				wayPoint = mPointList[i];
//			}
//		}
//	}
//
//	//�g�p�ς݂ɕύX
//	wayPoint->SetUseFlag(true);
//
//	return wayPoint;
//}
//
//void BaseEnemy::SerachWayPoint()
//{
//	//�񍐔͈͂ɓ������Ă��Ȃ� or �S�[���ɓ������Ă��� or �������񍐂��Ă���(isInvisible) �Ȃ珈�����Ȃ�
//	if (!hitReportArea || finishSearchWay || isInvincible) return;
//
//	loopCount++;
//
//	SearchPointToArrayTest(static_cast<int>(mTarget.size()));
//
//	resultPoint = NearWayPointArrayTest(mTarget, goalPoint)->GetPosition();
//
//	searchCount++;
//	moveList.push_back(resultPoint);
//
//	if (!finishSearchWay)
//	{
//		ClearFlag();
//	}
//}
//
//void BaseEnemy::SearchPointToArrayTest(int length)
//{
//	//��������߂��|�C���g��z��̐����擾����
//	for (int i = 0; i < length; i++)
//	{
//		mTarget[i] = NearWayPointStartTest(moveList[searchCount]);
//	}
//}
//
//shared_ptr<TestWayPoint> BaseEnemy::NearWayPointArrayTest(const vector<shared_ptr<TestWayPoint>>& trans, const Vector3 & goal)
//{
//	float distance = 0;
//	float mostDistance = 0;
//	shared_ptr<TestWayPoint> wayPoint;
//
//	for (int i = 0, end = static_cast<int>(mTarget.size()); i < end; i++)
//	{
//		Vector3 dist = goal - trans[i]->GetPosition();
//		distance = dist.Length();
//
//		//vector�ɂ́A�����ς݂̂��������Ȃ��͂�...
//		//if (mTarget[i]->GetUseFlag())
//		{
//			if (mostDistance == 0 || mostDistance > distance)
//			{
//				mostDistance = distance;
//				wayPoint = trans[i];
//			}
//		}
//
//		//�S�[���𔭌�������
//		if (trans[i]->GetPosition().x == goal.x &&
//			trans[i]->GetPosition().y == goal.y &&
//			trans[i]->GetPosition().z == goal.z)
//		{
//			finishSearchWay = true;
//			wayPoint = trans[i];
//			break;
//		}
//	}
//
//	if (!finishSearchWay)
//	{
//		//�S�[�����ĂȂ��Ƃ��͎g�p�ς݂ɂ���
//		wayPoint->SetUseFlag(true);
//	}
//
//	return wayPoint;
//}
//
//void BaseEnemy::ClearFlag()
//{
//
//	for (int i = 0, end = static_cast<int>(mPointList.size()); i < end; i++)
//	{
//		mPointList[i]->SetUseFlag(false);
//	}
//}

void BaseEnemy::WayPointMove()
{
	Vector3 otherPosition = moveList[moveCount];
	moveWayPoint = true;

	//�S�[���Ƃ̋������߂�������ړ����Ȃ��B
	if (!InsideDistance(otherPosition, 10.0f))
	{
		MovePoint(otherPosition);
	}
	else
	{
		InitWayPoint();
	}

	//���������Ƃ���
	if (InsideDistance(otherPosition, 8.0f))
	{
		if (static_cast<int>(moveList.size()) == moveCount + 1)
		{
			goalFlag = true;
			//InitWayPoint();
		}
		else
		{
			moveCount++;
		}
	}
}

//void BaseEnemy::Report(shared_ptr<ModelRenderer> modelRender)
//{
//	//���łɕ񍐂��Ă��� or ���݈ړ��� �Ȃ�\�����Ȃ�
//	if (isInvincible || moveWayPoint) return;
//
//	isInvincible = true;
//
//	//�U������@�\�ǉ�---------------------------------------------
//	DicideTurnAround();
//
//	//���S�����Ɉڂ�
//	if (HP <= 0)
//	{
//		mDeathFlag = true;
//		return;
//	}
//
//	//mReportArea = new ReportArea(position, objM, modelRender, number);
//	//mManager->Add(new ReportArea(position, mManager, modelRender, testNumber));
//	//testNumber++;
//}

void BaseEnemy::InitWayPoint()
{
	//�E�F�C�|�C���g�ړ����Ă��Ȃ������珈�����Ȃ�
	if (!moveWayPoint) return;

	moveList.clear();

	goalPoint = Vector3().zero;
	resultPoint = Vector3().zero;

	finishSearchWay = false;
	hitReportArea = false;
	moveWayPoint = false;
	goalFlag = false;

	//mTarget.clear();
	//mPointList.clear();

	searchCount = 0;
	loopCount = 0;
	moveCount = 0;
	arrayCount = 0;
}

void BaseEnemy::DicideTurnAround()
{
	hitPos = position;
	hitAngle = angle;

	////���G��ԂłȂ� or �U������@�\��OFF �Ȃ珈�������Ȃ�
	//if (mMoveState != MoveState::NOT_FIND || !TURNAROUND_MODE) return;

	//���������ʒu�ƁA���݂̎����̌������ꎞ�ۑ�����B
	hitPos = mPlayerPosition;
	hitAngle = angle;
	//mMoveState = MoveState::WARNING;
}

void BaseEnemy::TurnAround(int time)
{
	////�U������@�\��OFF or �v���C���[��ǂ��Ă��� or ������� �Ȃ珈�������Ȃ�
	//if (!TURNAROUND_MODE || mTrackingPlayer || isDestruct || mTrackingBreadcrumb || moveWayPoint) return;

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
	mFireAngle = (fanInfo.rotate + 90.0f);
}

void BaseEnemy::DestructMode(int hpLine, bool destructMode)
{
	//�������[�h��OFF �Ȃ珈�����Ȃ��B
	if (!destructMode) return;

	//�̗͂����ȉ��ɂȂ����玩�����[�h�ɂȂ�B
	if (HP <= hpLine)
	{
		mMoveState = MoveState::DESTRUCT;
	}
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

bool BaseEnemy::IsHitFanToPoint(const FanInfomation & fan, const Vector3 & point, const float radius) const
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

void BaseEnemy::SetBreadCreator(BreadCrumbCreater * breadCreator)
{
	/*mBreadCreator = breadCreator;*/
}

int BaseEnemy::GetHP() const
{
	return HP;
}

bool BaseEnemy::GetDeathFlag() const
{
	return mDeathFlag;
}

void BaseEnemy::SetEnemyAi(EnemyAI * enemyAI)
{
	mEnemyAI = enemyAI;
}

void BaseEnemy::SetObjectManager(ObjectManager * manager)
{
	mManager = manager;
}

void BaseEnemy::SetAttackTarget(const Vector3 & attackTarget)
{
	mAttackTarget = attackTarget;
}

void BaseEnemy::SetImportantObject(ObjectManager * manager, shared_ptr<ModelRenderer> modelRender, shared_ptr<ParticleManager> particleManager, shared_ptr<BreadCrumbCreater> breadCreator)
{
	mManager = manager;
	mRend = modelRender;
	mPart = particleManager;
	mBreadCreator = breadCreator;
}
