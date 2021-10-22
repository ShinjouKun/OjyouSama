#include "EnemyAI.h"
#include "TestWayPoint.h"
#include "WayPointManager.h"

//WayPointManager* EnemyAI::mManager = nullptr;

EnemyAI::EnemyAI(/*ObjectManager * objectManager*/const shared_ptr<WayPointManager>& m)
{
	//mObjectManager = objectManager;
	//Initialize();

	manager = m;
}

EnemyAI::~EnemyAI()
{
}

//void EnemyAI::Initialize()
//{
//	mPlayerPosition = Vector3().zero;
//}
//
//void EnemyAI::Update()
//{
//	//WatchPlayerPosition();//�v���C���[�̈ʒu���Ď�
//	//	//�G���ꊇ����
//	//for (int i = 0, end = mEnemyList.size(); i < end; i++)
//	//{
//	//	WatchEnemyAlive(i);//�G�̐�����Ԃ��Ď�
//	//
//	//	if (anyEnemyDeath)
//	//	{
//	//		break;
//	//	}
//	//
//	//	SwitchingEnemyActive(i);//�G�̕\����Ԃ̐؂�ւ�
//	//}
//}

//void EnemyAI::SetWayPointManager(WayPointManager * pointManager)
//{
//	mManager = pointManager;
//}

std::vector<Vector3> EnemyAI::GetMoveRoad(const Vector3& enemyPosition, const Vector3& destination)
{
	Vector3 goalPoint = destination;

	std::vector<Vector3> moveList;//�ړ����X�g

	mFindGoal = false;//�S�[�������������ǂ���

	int searchCount = 0;//����������

#pragma region �����������Ɉ�x�����s������

	//1:�ړI�n�̈ʒu�ɐV�����|�C���g�𐶐�
	shared_ptr<TestWayPoint> wayPoint = std::make_shared<TestWayPoint>(goalPoint);
	//mManager->AddTest(wayPoint);
	manager->AddTest(wayPoint);

	//2:�t�B�[���h�ɂ���|�C���g��S�Ď擾
	//2-1:�|�C���g�̃t���O��������
	//mManager->ResetFlag();
	manager->ResetFlag();
	//mPointList.resize(mManager->GetTestPointList().size());//�ŏ��ɃT�C�Y�̕ύX
	mPointList.resize(manager->GetTestPointList().size());//�ŏ��ɃT�C�Y�̕ύX
	//2-2:���X�g�ɃR�s�[(�d��)
	//mPointList = mManager->GetTestPointList();
	mPointList = manager->GetTestPointList();
	//2-3:�ړ����X�g�̃��������m��
	moveList.reserve(mPointList.size());
	//2-4:�s�K�v�ȃ������̍폜
	//mManager->Remove();//��1�Œǉ�������́A�����g��Ȃ��̂ō폜����
	manager->Remove();//��1�Œǉ�������́A�����g��Ȃ��̂ō폜����
	wayPoint.reset();

	//3:���g����߂��|�C���g���i�[���郊�X�g�𐶐�
	vector<shared_ptr<TestWayPoint>> nearPoints(4);

	//4:�ŏ��̌������s���ʒu������
	moveList.insert(moveList.begin(), enemyPosition);

#pragma endregion

#pragma region ���[�g������

	//�S�[���𔭌��ł���܂Ń��[�v����
	while (!mFindGoal)
	{
		//1:�����̈ʒu����A�ł��߂��|�C���g���w�萔�擾
		for (int i = 0, end = nearPoints.size(); i < end; i++)
		{
			//��������ʒu�́A���݂���|�C���g
			nearPoints[i] = NearWayPointToNowPosition(moveList[searchCount]);
		}

		//2:���g����߂��|�C���g�̒�����A�S�[���ɍł��߂��|�C���g��1�擾
		Vector3 resultPosition = NearWayPointToGoal(nearPoints, goalPoint)->GetPosition();

		//3:�w���񐔂𑝂₷
		searchCount++;

		//4:�ړ����X�g�Ɋi�[����
		moveList.push_back(resultPosition);
	}

#pragma endregion

	return moveList;
}

shared_ptr<TestWayPoint> EnemyAI::NearWayPointToNowPosition(const Vector3 & nowPosition)
{
	float distance = 0;    //�ΏۂƂ̋���
	float mostDistance = 0;//�ł��߂�����
	shared_ptr<TestWayPoint> wayPoint;

	//�|�C���g���X�g��S�������āA�ł��߂��|�C���g��1�擾����
	for (int i = 0, end = mPointList.size(); i < end; i++)
	{
		//�ΏۂƂ̋������v�Z
		Vector3 dist = mPointList[i]->GetPosition() - nowPosition;
		distance = dist.Length();

		//�܂��g�p���Ă��Ȃ����̂̂ݏ������s��
		if (!mPointList[i]->GetUseFlag())
		{
			if (mostDistance == 0 || mostDistance > distance)
			{
				mostDistance = distance;
				wayPoint = mPointList[i];
			}
		}
	}

	//�g�p�ς݂ɕύX-----------------------------------�G���[�|�C���g------------------------------
	wayPoint->SetUseFlag(true);

	return wayPoint;
}

shared_ptr<TestWayPoint> EnemyAI::NearWayPointToGoal(const vector<shared_ptr<TestWayPoint>>& nearPoints, const Vector3 & goalPosition)
{
	float distance = 0;    //�ΏۂƂ̋���
	float mostDistance = 0;//�ł��߂�����
	shared_ptr<TestWayPoint> wayPoint;

	for (int i = 0, end = nearPoints.size(); i < end; i++)
	{
		//�S�[���ƈʒu����v����|�C���g��T��
		if (nearPoints[i]->GetPosition().x == goalPosition.x &&
			nearPoints[i]->GetPosition().y == goalPosition.y &&
			nearPoints[i]->GetPosition().z == goalPosition.z)
		{
			//�S�[���𔭌������珈�����I���
			wayPoint = nearPoints[i];
			mFindGoal = true;
			break;
		}

		//�ΏۂƂ̋������v�Z
		Vector3 dist = nearPoints[i]->GetPosition() - goalPosition;
		distance = dist.Length();

		if (mostDistance == 0 || mostDistance > distance)
		{
			mostDistance = distance;
			wayPoint = nearPoints[i];
		}
	}

	//if (!finishSearchWay)
	//{
	//	//�S�[�����ĂȂ��Ƃ��͎g�p�ς݂ɂ���
	//	wayPoint->SetUseFlag(true);
	//}

	return wayPoint;
}

//void EnemyAI::WatchPlayerPosition()
//{
//	//�v���C���[�̈ʒu���擾
//	mPlayerPosition = mObjectManager->GetPlayer().GetPosition();
//	float debugPlayerPosition[3] = { mPlayerPosition.x,mPlayerPosition.y,mPlayerPosition.z };
//	ImGui::SliderFloat3("PlayerPosition----------", debugPlayerPosition, -1000.0f, 1000.0f);
//}
//
//void EnemyAI::WatchEnemyAlive(const int count)
//{
//	int debugHP = mEnemyList[count]->GetHP();
//	ImGui::SliderInt("HP", &debugHP, 0, 100);
//
//	anyEnemyDeath = false;
//
//	if (mEnemyList[count]->GetHP() <= 0)
//	{
//		mEnemyList.erase(mEnemyList.begin() + count);
//
//		/*���̂܂ܑ�����ƁA�w��ԍ��̗v�f���Ȃ����Č����邩��A
//		�@���O�ɏo�Ă���A������x���[�v���s��*/
//		anyEnemyDeath = true;
//	}
//}
//
//void EnemyAI::SwitchingEnemyActive(const int count)
//{
//	//�v���C���[�Ƃ̋��������ȉ��Ȃ�
//	if (SearchPlayer(mEnemyList[count]->GetPosition(), ACTIVE_DISTANCE))
//	{
//		//�\��
//		mEnemyList[count]->SetActive(true);
//	}
//	else//�����łȂ��Ȃ�
//	{
//		//��\��
//		mEnemyList[count]->SetActive(false);
//	}
//}
//
//bool EnemyAI::SearchPlayer(const Vector3& enemyPosition, const float dist) const
//{
//	//�v���C���[�Ǝ����̋������v��
//	Vector3 distance = mPlayerPosition - enemyPosition;
//	float length = distance.Length();
//
//	ImGui::SliderFloat("Distance2Player", &length, -1000.0f, 1000.0f);
//
//	if (length > dist)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//Vector3 EnemyAI::GetPlayerPosition() const
//{
//	return mPlayerPosition;
//}