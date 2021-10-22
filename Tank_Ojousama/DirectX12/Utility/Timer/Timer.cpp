#include "Timer.h"

Timer::Timer(float t):
	mLimitTime(t),
	mCurrentTime(0.0f)
{
}

Timer::~Timer() = default;

void Timer::update()
{
	mCurrentTime += Timer::mDeltaTime;
}

void Timer::setTime(float t)
{
	mLimitTime = t;
	mCurrentTime = 0.0f;
}

bool Timer::isTime()
{
	return mCurrentTime >= mLimitTime;
}

float Timer::rate() const
{
	return mCurrentTime / mLimitTime;
}

float Timer::getCuttentTime() const
{
	return mCurrentTime;
}

float Timer::mDeltaTime = 0.01666f;