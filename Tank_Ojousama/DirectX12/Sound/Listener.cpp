#include "Listener.h"
#include "SoundSystem.h"
#include "3D/Sound3DListener/Sound3DListener.h"

Listener::Listener()
{
	mListener = std::make_shared<Sound3DListener>();
	auto& s = SoundSystem::instance();
	s.setListener(mListener);
}

Listener::~Listener()
{
}

void Listener::setPos(const Vector3 & pos)
{
	mListener->setPosition(pos);
}
