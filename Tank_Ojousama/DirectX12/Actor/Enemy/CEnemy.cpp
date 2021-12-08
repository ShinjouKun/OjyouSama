#include "CEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/ElfBullet.h"
#include "../../Utility/Timer/Timer.h"
#include "MemberEnemy.h"

CEnemy::CEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	int num,
	bool advanceFlag,
	const Vector3& advanceDirection
)
{
	position = pos;
	angle = ang;
	mOriginNumber = num;
	mAdvanceFlag = advanceFlag;
	mAdvanceDirection = advanceDirection;
}

CEnemy::~CEnemy()
{
}

void CEnemy::EnemyInit()
{
	HP = 10;
	attackTime = 60;

	speed = 0.1f;
	mRadius = 1.0f;
	mSwingRange = 45.0f;
	mFireAngle = angle.y;
	//turretAngle = 0.0f;
	mAttackLength = 10.0f;

	death = false;
	breadcrumbMode = true;
	DESTRUCT_MODE = false;
	TURNAROUND_MODE = false;
	RECEIVEREPORT_MODE = false;

	mOnlyOnceTrigger = false;
	mSearchCommand = false;
	mSearchResult = false;

	scale = Vector3(1.0f, 1.0f, 1.0f);
	mSearchPosition = Vector3().zero;

	objType = ObjectType::ENEMY;
	//SetCollidder(new SphereCollider(position, radius));
	SetCollidder(new SphereCollider(Vector3().zero, mRadius));

	//�Z���T�[�̏�����----------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//�ʒu
	fanInfo.fanRange = 180.0f;									   //�Ƃ̊p�x
	fanInfo.length = 30.0f;										   //����
	fanInfo.rotate = mFanRotateOrigin;							   //��]�p
	//--------------------------------

	mMoveState = MoveState::NOT_FIND;
	searchStep = SearchCommandStep::SEARCH_ORDER;

	mMemberList.resize(MEMBER_COUNT);
	mMemberPositionList.resize(MEMBER_COUNT);
	mEnemyNumber = 0;

	mSearchTimer = std::make_shared<Timer>();
	mSearchTimer->setTime(SEARCH_INTERVAL);

#pragma region ���f���̓ǂݍ���

	//�ԍ���int����string�ɕϊ�
	mMyNumber = to_string(mOriginNumber);

	//�E�����o�^
	mRightLeg =  "CaptainLegR";
	mRLegNumber = mRightLeg + mMyNumber;
	mRend->AddModel(mRLegNumber,  "Resouse/EnemyModel/Elf_C/leg_R_C.obj", "Resouse/EnemyModel/Elf_C/leg_LR2.png");
	mRend->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�������o�^
	mLeftLeg = "CaptainLegL";
	mLLegNumber = mLeftLeg + mMyNumber;
	mRend->AddModel(mLLegNumber, "Resouse/EnemyModel/Elf_C/leg_L_C.obj", "Resouse/EnemyModel/Elf_C/leg_LR2.png");
	mRend->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�����o�^
	mHead = "CaptainHead";
	mHeadNumber = mHead + mMyNumber;
	mRend->AddModel(mHeadNumber, "Resouse/EnemyModel/Elf_C/elf_head3.obj", "Resouse/EnemyModel/Elf_C/face_color3.png");

	//�̏��o�^
	mBody = "CaptainBody";
	mBodyNumber = mBody + mMyNumber;
	mRend->AddModel(mBodyNumber, "Resouse/EnemyModel/Elf_C/elf_body3.obj", "Resouse/EnemyModel/Elf_C/hand_bow_color3.png");

#pragma endregion
}

void CEnemy::EnemyUpdate()
{
	/*�����𐶐�*/
	OnlyOnceAction();

	/*���ʂ̗v�f*/
	ChangeState(); //��ԕύX
	SearchObject();//�p��������v���C���[��T��

	if (HP <= 0)
	{
		for (auto& list : mMemberList)
		{
			list->ReceiveCaptainLost(true);
		}
	}

	if (moveFlag)
	{
		if (mRotDirection)
		{
			mLegRotate += LEG_SPEED;
			if (mLegRotate > LEG_RANGE)
			{
				mRotDirection = false;
			}
		}
		else
		{
			mLegRotate -= LEG_SPEED;
			if (mLegRotate < -LEG_RANGE)
			{
				mRotDirection = true;
			}
		}
	}
	else
	{
		mLegRotate = 0.0f;
	}


	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		//int debugHP = mMemberList[i]->GetHP();
        //ImGui::SliderInt("HP-------------------", &debugHP, 0, 500);

		mAnyDeathFlag = false;

		//������Ԃ��Ď�
		bool deadFlag = mMemberList[i]->GetDeadFlag();

		if (deadFlag)
		{
			mMemberList.erase(mMemberList.begin() + i);

			mAnyDeathFlag = true;
		}

		if (mAnyDeathFlag) break;

		/*�ړ��Ǘ�*/
		MemberMove(i);
	}

	//���G�͈͓��Ƀv���C���[�����邩
	if (InsideDistance(mPlayerPosition, MEMBER_DISTANCE))
	{
		for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
		{
			mMemberList[i]->ReceiveAttackCommand(mPlayerPosition, true);
		}

		mSearchCommand = false;
		mSearchResult = false;
	}
	else//�����̍��G�͈͓��Ƀv���C���[�����Ȃ��Ƃ�
	{
		//���G�w�߂��o�Ă��Ȃ���
		if (!mSearchCommand)
		{
			//2�b��1����G�w�߂��o��
			mSearchTimer->update();

			if (mSearchTimer->isTime())
			{
				//���G�w�߂��o��
				for (const auto& list : mMemberList)
				{
					list->ReceiveSearchCommand(mPlayerPosition, true);
				}

				mSearchTimer->setTime(SEARCH_INTERVAL);
				mSearchCommand = true;
			}
		}

		//���G�w�߂��o�Ă���
		if (mSearchCommand)
		{
			for (const auto& list : mMemberList)
			{
				//���G���ʂ��󂯎��
				mSearchResult = list->SendSearchResult();
				//�Ώۂ𔭌����Ă�����
				if (mSearchResult)
				{
					//�Ώۂ̈ʒu����x�����擾
					mSearchPosition = list->SendSearchPosition();
					break;
				}
			}

			//�S���̕񍐂��󂯎�� or �N��1�l���Ώۂ𔭌����Ă�����񍐏I��
			mSearchCommand = false;
		}

		//�Ώۂ𔭌����Ă�����
		if (mSearchResult)
		{
			//�߂��Ȃ����猳�ɖ߂�
			if (InsideDistance(mSearchPosition, 1.0f))
			{
				mSearchResult = false;
			}
			else
			{
				//�Ώۂ̈ʒu�܂ňړ�
				MovePoint(mSearchPosition);
			}
		}
	}

	if (mAttackFlag)
	{
		attackCount++;

		if (attackCount > attackTime)
		{
			attackCount = 0;
			Vector3 firePos = AngleToVectorY(fanInfo.rotate);

			//�e�𔭎ˁI�I
			mManager->Add(new ElfBullet(position + firePos, Vector3(0, -angle.y, 0), mManager, mRend, mPart, objType, bulletNumber));
			bulletNumber++;
			mAttackFlag = false;
			mMoveState = MoveState::NOT_FIND;
		}
	}
	else
	{
		TrackingObject();//�ړ��֘A
	}

	//ImGui::Checkbox("SearchCommand", &mSearchCommand);
	//ImGui::Checkbox("SearchResult", &mSearchResult);

	//float time = mSearchTimer->getCuttentTime();
	//ImGui::SliderFloat("SearchTimer", &time, 0, time);

	////�v���C���[�̈ʒu���擾
	//float debugPlayerPosition[3] = { mPlayerPosition.x,mPlayerPosition.y,mPlayerPosition.z };
	//ImGui::SliderFloat3("PlayerPosition----------", debugPlayerPosition, -1000.0f, 1000.0f);
}

void CEnemy::EnemyRend()
{
	//���f���̕`��
	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);//�E�r
	mRend->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);//���r
	mRend->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//���Ǝ�
	mRend->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//�̂Ƌ|	
}

void CEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//�_���[�W���󂯂�
		HP -= col->GetColObject()->GetDamage();
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}

	if (mAdvanceFlag && col->GetColObject()->GetType() == ObjectType::BORDER_LINE)
	{
		mHitBorderLine = true;
	}
}

void CEnemy::EnemyImGuiDebug()
{
}

void CEnemy::Search()
{
	//SwingDirection(swingRange);
}

void CEnemy::Warning()
{
	//TrackingObject();
}

void CEnemy::Attack()
{
	//attackCount++;

	////���b���Ƃɓ����蔻��������̑O���ɕ\������
	//if (attackCount > attackTime)
	//{
	//	attackCount = 0;

	//	//�U�����I�������x����Ԃɖ߂�
	//	actionState = ActionState::WARNING;
	//}
}

void CEnemy::OnlyOnceAction()
{
	//���łɎ��s���Ă����� �������Ȃ�
	if (mOnlyOnceTrigger) return;

	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		Vector3 pos = position + AngleToVectorY(360.0f / mMemberList.size() * i) * MEMBER_DISTANCE;
		auto enemys = new MemberEnemy(pos, angle, mManager, mRend, mPart, mEnemyNumber++);
		mMemberList[i] = enemys;
		mManager->Add(enemys);
	}

	//�������I�������t���O�����Ă�
	mOnlyOnceTrigger = true;
}

void CEnemy::MemberMove(int number)
{
	//���ꂽ�ʒu�Ƀ|�W�V�������w��
	mMemberPositionList[number] = position + AngleToVectorY(360.0f / mMemberList.size() * number) * MEMBER_DISTANCE;

	//�����Ɉʒu����n���B
	mMemberList[number]->ReceivePosition(mMemberPositionList[number]);
}
