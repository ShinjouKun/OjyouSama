#pragma once
#include<string>
#include"../../Math/Math.h"
using namespace std;

namespace GolemEnemyConstInfo
{
	/*�ő�̗�*/
	constexpr int MAX_HP = 30;

	/*��b�U����*/
	constexpr int ATTACK = 10;

	/*�������U����*/
	constexpr int ATTACK_R = 5;

	/*���_�|�C���g�ɖ߂�܂ł̎���(�b)*/
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
	constexpr float FAN_LENGTH = 80.0f;

	/*���a*/
	constexpr float RADIUS = 1.0f;

	/*�U���͈�*/
	constexpr float ATTACK_LENGTH = 30.0f;

	/*��U��p�x*/
	constexpr float SWING_RANGE = 90.0f;
	/*�������[�h*/
	constexpr bool DESTRUCT_MODE = true;
	/*�p�������ǐՃ��[�h*/
	constexpr bool BRRADCRUMB_MODE = true;
	/*�U��Ԃ胂�[�h*/
	constexpr bool TURNAROUND_MODE = false;

	/*���_�|�C���g*/
	const Vector3 CAMP_POINT = Vector3(100.0f, 4.0f, -120.0f);

	/*�傫��*/
	const Vector3 SCALE = Vector3(3.0f, 3.0f, 3.0f);

	/*���̃��f����*/
	const string GOLEM_BODY_NAME = "GolemBody";
	/*�r���f����*/
	const string GOLEM_R_ARM_NAME = "GolemARMR";
	const string GOLEM_L_ARM_NAME = "GolemARML";
}