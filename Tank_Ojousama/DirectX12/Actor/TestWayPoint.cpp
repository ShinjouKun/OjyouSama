#include "TestWayPoint.h"

TestWayPoint::TestWayPoint(const Vector3 & pos):
	mPosition(std::make_shared<Vector3>(0,0,0))

{
	mPosition->x = pos.x;
	mPosition->y = pos.y;
	mPosition->z = pos.z;

	FlagInit();
}

TestWayPoint::~TestWayPoint()
{
}

void TestWayPoint::FlagInit()
{
	//�e�t���O�̏�����
	useFlag = false;
	arrivalFlag = false;
	impossibleFlag = false;
	closeFlag = false;
}

Vector3 TestWayPoint::GetPosition()
{
	//�X�}�[�g�|�C���^��Get�Ȃǂœn���ۂ́A*������
	return *mPosition;
}

bool TestWayPoint::GetUseFlag()
{
	return useFlag;
}

bool TestWayPoint::GetImpossibleFlag()
{
	return impossibleFlag;
}

bool TestWayPoint::GetArrivalFlag()
{
	return false;
}

bool TestWayPoint::GetCloseFlag()
{
	return false;
}

void TestWayPoint::SetUseFlag(const bool value)
{
	useFlag = value;
}

void TestWayPoint::SetCloseFlag(const bool value)
{
	closeFlag = value;
}

void TestWayPoint::SetImpossibleFlag(const bool value)
{
	impossibleFlag = value;
}
