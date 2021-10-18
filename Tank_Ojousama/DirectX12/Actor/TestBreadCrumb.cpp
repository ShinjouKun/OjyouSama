#include "TestBreadCrumb.h"

TestBreadCrumb::TestBreadCrumb(const Vector3 & position, int breadNumber)
{
	mPosition = position;
	mBreadNumber = breadNumber;
	Initialize();
}

TestBreadCrumb::~TestBreadCrumb()
{
}

void TestBreadCrumb::DeathInterval()
{
	if (!mDeathFlag)
	{
		lifeCount++;

		if (lifeCount > lifeTime)
		{
			lifeCount = 0;
			mDeathFlag = true;
		}
	}
}

Vector3 TestBreadCrumb::GetPosition() const
{
	return mPosition;
}

int TestBreadCrumb::GetBreadNumber() const
{
	return mBreadNumber;
}

bool TestBreadCrumb::GetDeathFlag() const
{
	return mDeathFlag;
}

void TestBreadCrumb::Initialize()
{
	lifeTime = 3 * 60;
	lifeCount = 0;
	mDeathFlag = false;
}
