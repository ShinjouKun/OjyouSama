#pragma once
#include <string>
#include "../../Math/Math.h"

using namespace std;

namespace SniperEnemyConstInfo
{
	/*�ő�̗�*/
	constexpr int MAX_HP = 10;

	/*�U����*/
	constexpr int ATTACK = 1;

	/*���G�ɖ߂�܂ł̎���(�b)*/
	constexpr int WARNING_TIME = 1;

	/*�U���Ԋu(�b)*/
	constexpr int ATTACK_TIME = 1;

	/*���G����(�b)*/
	constexpr int INVINCIBLE_TIME = 5;

	/*�ړ����x*/
	constexpr float MOVE_SPEED = 0.2f;

	/*���G�͈͂̊p�x*/
	constexpr float FAN_RANGE = 180.0f;

	/*���G�͈͂̋���*/
	constexpr float FAN_LENGTH = 50.0f;

	/*���a*/
	constexpr float RADIUS = 2.0f;

	/*�U���͈�*/
	constexpr float ATTACK_LENGTH = 30.0f;

	/*��U��p�x*/
	constexpr float SWING_RANGE = 90.0f;

	/*�傫��*/
	const Vector3 SCALE = Vector3(2.5f, 2.5f, 2.5f);

	/*�C�g�̃��f����*/
	const string TANK_BARREL_NAME = "SniperBarrel";

	/*�C���̃��f����*/
	const string TANK_TURRET_NAME = "SniperTurret";

	/*�ԑ̂̃��f����*/
	const string TANK_BODY_NAME = "SniperBody";
}