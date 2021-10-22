#pragma once

class Timer
{
public:
	Timer(float t = 0.f);
	~Timer();
	void update();
	void setTime(float t);
	bool isTime() const;
	float rate()const;
	float getCuttentTime()const;

public:
	static float mDeltaTime;

private:
	float mLimitTime;
	float mCurrentTime;
};