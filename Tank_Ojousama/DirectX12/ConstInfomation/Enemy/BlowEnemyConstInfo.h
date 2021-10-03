#pragma once
#include <string>
#include "../../Math/Math.h"

using namespace std;

namespace BlowEnemyConstInfo
{
	/*�ő�̗�*/
	constexpr int MAX_HP = 2;

	/*�U����*/
	constexpr int ATTACK = 1;

	/*���G�ɖ߂�܂ł̎���(�b)*/
	constexpr int WARNING_TIME = 1;

	/*�U���Ԋu(�b)*/
	constexpr int ATTACK_TIME = 1;

	/*���G����(�b)*/
	constexpr int INVINCIBLE_TIME = 2;

	/*�ړ����x*/
	constexpr float MOVE_SPEED = 0.2f;

	/*���G�͈͂̊p�x*/
	constexpr float FAN_RANGE = 180.0f;

	/*���G�͈͂̋���*/
	constexpr float FAN_LENGTH = 30.0f;

	/*���a*/
	constexpr float RADIUS = 1.0f;

	/*�U���͈�*/
	constexpr float ATTACK_LENGTH = 8.0f;

	/*��U��p�x*/
	constexpr float SWING_RANGE = 90.0f;

	/*�傫��*/
	const Vector3 SCALE = Vector3(1.5f, 1.5f, 1.5f);

	/*�C�g�̃��f����*/
	const string TANK_BARREL_NAME = "Barrel";

	/*�C���̃��f����*/
	const string TANK_TURRET_NAME = "Turret";

	/*�ԑ̂̃��f����*/
	const string TANK_BODY_NAME = "Body";
}