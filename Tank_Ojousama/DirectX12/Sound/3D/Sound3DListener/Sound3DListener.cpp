#include "Sound3DListener.h"

Sound3DListener::Sound3DListener():
	mListener(),
	mPreviousPos(Vector3::zero)
{
	mListener.cone = &LISTENER_DIRECTIONAL_CONE;
}

Sound3DListener::~Sound3DListener() = default;

void Sound3DListener::update()
{
	mListener.velocity = (mListener.position - mPreviousPos);
	mPreviousPos = mListener.position;
}

void Sound3DListener::setPosition(const Vector3 & pos)
{
	mListener.position = pos;
}

void Sound3DListener::setFrontAndTop(const Vector3 & front, const Vector3 & top)
{
	mListener.orientFront = front;
	mListener.orientTop = top;
}

void Sound3DListener::setCone(Sound3DCone * cone)
{
	mListener.cone = cone;
}

const Sound3DListenerParam & Sound3DListener::getListener() const
{
	return mListener;
}

Sound3DCone Sound3DListener::LISTENER_DIRECTIONAL_CONE = { Math::PI * 5.f / 6.f, Math::PI * 11.f / 6.f, 1.f, 0.75f, 0.f, 0.25f, 0.708f, 1.f };