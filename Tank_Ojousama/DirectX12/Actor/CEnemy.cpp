#include "CEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../Utility/Timer/Timer.h"
#include "MemberEnemy.h"

CEnemy::CEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<ParticleManager> effectManager,
	int num
)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	mModelRender = modelRender;
	mEffectManager = effectManager;
	number = num;
}

CEnemy::~CEnemy()
{
}

void CEnemy::EnemyInit()
{
	HP = 10;
	attackTime = 60;

	speed = 0.1f;
	radius = 1.0f;
	swingRange = 45.0f;
	barrelAngle = angle.y;
	turretAngle = 0.0f;
	attackLength = 10.0f;

	death = false;
	breadcrumbMode = true;
	DESTRUCT_MODE = false;
	TURNAROUND_MODE = false;
	RECEIVEREPORT_MODE = false;

	mOnlyOnceTrigger = false;
	mSearchCommand = false;
	mSearchResult = false;

	scale = Vector3(2.0f, 2.0f, 2.0f);
	mSearchPosition = Vector3().zero;

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(position, radius));

	//�Z���T�[�̏�����----------------
	fanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//�ʒu
	fanInfo.fanRange = 180.0f;									   //�Ƃ̊p�x
	fanInfo.length = 30.0f;										   //����
	fanInfo.rotate = fanRotateOrigin;							   //��]�p
	//--------------------------------

	actionState = ActionState::SEARCH;
	searchStep = SearchCommandStep::SEARCH_ORDER;

	mMemberList.resize(MEMBER_COUNT);
	mMemberPositionList.resize(MEMBER_COUNT);
	mEnemyNumber = 0;

	mSearchTimer = std::make_shared<Timer>();
	mSearchTimer->setTime(SEARCH_INTERVAL);

#pragma region ���f���̓ǂݍ���

	//��Ԃ̖C�g(�㉺�Ɉړ����镔��)Barrel
	tankBarrel = "Barrel";
	num = to_string(number);
	numBarrel = tankBarrel + num;
	mModelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	mModelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//���S�_�̕ύX

	//��Ԃ̖C��(��̕���)Turret
	tankTurret = "Turret";
	num = to_string(number);
	numTurret = tankTurret + num;
	mModelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	mModelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//��Ԃ̎ԑ�(���̕���)Body
	tankBody = "Body";
	num = to_string(number);
	numBody = tankBody + num;
	mModelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	mModelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));

	mCircle = "Circle";
	num = to_string(number);
	mCircleNumber = mCircle + num;
	mModelRender->AddModel(mCircleNumber, "Resouse/maru.obj", "Resouse/select.png");

#pragma endregion
}

void CEnemy::EnemyUpdate()
{
	/*�����𐶐�*/
	OnlyOnceAction();

	if (HP <= 0)
	{
		for (auto& list : mMemberList)
		{
			list->ReceiveCaptainLost(true);
		}
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
				Move(mSearchPosition);
			}
		}

		////�w�肵�������ɓ���������B
		//Move(position + Vector3(0.1f, 0.0f, 0.0f));
	}

	ImGui::Checkbox("SearchCommand", &mSearchCommand);
	ImGui::Checkbox("SearchResult", &mSearchResult);

	float time = mSearchTimer->getCuttentTime();
	ImGui::SliderFloat("SearchTimer", &time, 0, time);

	////�v���C���[�̈ʒu���擾
	//float debugPlayerPosition[3] = { mPlayerPosition.x,mPlayerPosition.y,mPlayerPosition.z };
	//ImGui::SliderFloat3("PlayerPosition----------", debugPlayerPosition, -1000.0f, 1000.0f);
}

void CEnemy::EnemyRend()
{
	//���f���̕`��
	DirectXManager::GetInstance()->SetData3D();
	mModelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	mModelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	mModelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
	mModelRender->Draw(mCircleNumber, Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), Vector3(10, 1, 10));
}

void CEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//�_���[�W���󂯂�
		HP -= col->GetColObject()->GetDamage();
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
	TrackingObject();
}

void CEnemy::Attack()
{
	attackCount++;

	//���b���Ƃɓ����蔻��������̑O���ɕ\������
	if (attackCount > attackTime)
	{
		attackCount = 0;

		//�U�����I�������x����Ԃɖ߂�
		actionState = ActionState::WARNING;
	}
}

void CEnemy::Destruct()
{
}

void CEnemy::OnlyOnceAction()
{
	//���łɎ��s���Ă����� �������Ȃ�
	if (mOnlyOnceTrigger) return;

	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		auto enemys = new MemberEnemy(mMemberPositionList[i], angle, mObjManager, mModelRender, mEffectManager, mEnemyNumber++);
		mMemberList[i] = enemys;
		mObjManager->Add(enemys);
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
