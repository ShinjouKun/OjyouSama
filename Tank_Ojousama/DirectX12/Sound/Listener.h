#pragma once

#include "../Math/Math.h"
#include <memory>

class Sound3DListener;

class Listener
{
public:
	Listener();
	~Listener();

	//
	void setPos(const Vector3& pos);

private:
	Listener(const Listener&) = delete;
	Listener& operator=(const Listener&) = delete;

private:
	std::shared_ptr<Sound3DListener> mListener;

};