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

Vector3 TestWayPoint::GetPosition() const
{
	//�X�}�[�g�|�C���^��Get�Ȃǂœn���ۂ́A*������
	return *mPosition;
}

bool TestWayPoint::GetUseFlag() const
{
	return useFlag;
}

bool TestWayPoint::GetImpossibleFlag() const
{
	return impossibleFlag;
}

bool TestWayPoint::GetArrivalFlag() const
{
	return arrivalFlag;
}

bool TestWayPoint::GetCloseFlag() const
{
	return closeFlag;
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
