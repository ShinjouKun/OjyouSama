#pragma once
#include<string>
#include"../../Math/Math.h"
using namespace std;

namespace GolemEnemyConstInfo
{
	/*最大体力*/
	constexpr int MAX_HP = 30;

	/*基礎攻撃力*/
	constexpr int ATTACK = 10;

	/*遠距離攻撃力*/
	constexpr int ATTACK_R = 5;

	/*拠点ポイントに戻るまでの時間(秒)*/
	constexpr int WARNING_TIME = 1;

	/*攻撃間隔(秒)*/
	constexpr int ATTACK_TIME = 1;

	/*無敵時間(秒)*/
	constexpr int INVINCIBLE_TIME = 2;

	/*移動速度*/
	constexpr float MOVE_SPEED = 0.2f;

	/*索敵範囲の角度*/
	constexpr float FAN_RANGE = 180.0f;

	/*索敵範囲の距離*/
	constexpr float FAN_LENGTH = 80.0f;

	/*半径*/
	constexpr float RADIUS = 1.0f;

	/*攻撃範囲*/
	constexpr float ATTACK_LENGTH = 30.0f;

	/*首振り角度*/
	constexpr float SWING_RANGE = 90.0f;
	/*自爆モード*/
	constexpr bool DESTRUCT_MODE = true;
	/*パンくず追跡モード*/
	constexpr bool BRRADCRUMB_MODE = true;
	/*振り返りモード*/
	constexpr bool TURNAROUND_MODE = false;

	/*拠点ポイント*/
	const Vector3 CAMP_POINT = Vector3(100.0f, 4.0f, -120.0f);

	/*大きさ*/
	const Vector3 SCALE = Vector3(3.0f, 3.0f, 3.0f);

	/*胴体モデル名*/
	const string GOLEM_BODY_NAME = "GolemBody";
	/*腕モデル名*/
	const string GOLEM_R_ARM_NAME = "GolemARMR";
	const string GOLEM_L_ARM_NAME = "GolemARML";
}