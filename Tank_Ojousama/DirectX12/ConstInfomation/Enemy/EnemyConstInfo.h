#pragma once
#include "../../Math/Math.h"

namespace EnemyConstInfo
{
	/*最大体力*/
	constexpr int MAX_HP = 1;

	/*攻撃力*/
	constexpr int ATTACK = 1;

	/*索敵に戻るまでの時間(秒)*/
	constexpr int WARNING_TIME = 1;

	/*攻撃間隔(秒)*/
	constexpr int ATTACK_TIME = 1;

	/*移動速度*/
	constexpr float MOVE_SPEED = 1.0f;

	/*半径*/
	constexpr float RADIUS = 1.0f;

	/*攻撃範囲*/
	constexpr float ATTACK_LENGTH = 1.0f;

	/*首振り角度*/
	constexpr float SWING_RANGE = 1.0f;

	/*報告クールタイム*/
	constexpr float REPORT_INTERVAL = 5.0f;

	/*パンくず追跡モード*/
	constexpr bool BRRADCRUMB_MODE = true;

	/*自爆モード*/
	constexpr bool DESTRUCT_MODE = false;

	/*振り返りモード*/
	constexpr bool TURNAROUND_MODE = false;

	/*大きさ*/
	const Vector3 SCALE = Vector3(1.0f, 1.0f, 1.0f);
}