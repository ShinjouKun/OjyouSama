#pragma once
#include "../../Math/Math.h"

namespace EnemyConstInfo
{
	/*�ő�̗�*/
	constexpr int MAX_HP = 1;

	/*�U����*/
	constexpr int ATTACK = 1;

	/*���G�ɖ߂�܂ł̎���(�b)*/
	constexpr int WARNING_TIME = 1;

	/*�U���Ԋu(�b)*/
	constexpr int ATTACK_TIME = 1;

	/*�ړ����x*/
	constexpr float MOVE_SPEED = 1.0f;

	/*���a*/
	constexpr float RADIUS = 1.0f;

	/*�U���͈�*/
	constexpr float ATTACK_LENGTH = 1.0f;

	/*��U��p�x*/
	constexpr float SWING_RANGE = 1.0f;

	/*�p�������ǐՃ��[�h*/
	constexpr bool BRRADCRUMB_MODE = true;

	/*�������[�h*/
	constexpr bool DESTRUCT_MODE = false;

	/*�U��Ԃ胂�[�h*/
	constexpr bool TURNAROUND_MODE = false;

	/*�傫��*/
	const Vector3 SCALE = Vector3(1.0f, 1.0f, 1.0f);
}