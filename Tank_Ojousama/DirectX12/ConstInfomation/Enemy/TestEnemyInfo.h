#pragma once
#include "../../Math/Math.h"

namespace TestEnemyInfo
{
	constexpr int MAX_HP = 5;
	constexpr int ATTACK = 3;

	constexpr int WARNING_TIME = 3;//�x������(�b)
	constexpr int ATTACK_TIME = 1; //�U������(�b)

	constexpr float RADIUS = 1.0f;           //�����蔻��̔��a
	constexpr float ATTACK_LENGTH = 5.0f;   //�U���͈�

	const Vector3 SCALE = Vector3(1.5f, 1.5f, 1.5f);       //�傫��(1.5)
}