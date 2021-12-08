#pragma once
#include <memory>

class IParticle
{
public:
	virtual ~IParticle() = default;
	virtual void update() = 0;

};