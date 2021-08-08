#include "AABBCollider.h"
#pragma once

AABBCollider::~AABBCollider()
{
}

void AABBCollider::Update()
{
	AABB::min = object->GetPosition();
	AABB::max = max;
}
