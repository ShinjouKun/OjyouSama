#pragma once
#include <string>
#include "../../Math/Math.h"

using namespace std;

namespace SniperEnemyConstInfo
{
	/*Å‘å‘Ì—Í*/
	constexpr int MAX_HP = 10;

	/*UŒ‚—Í*/
	constexpr int ATTACK = 1;

	/*õ“G‚É–ß‚é‚Ü‚Å‚ÌŠÔ(•b)*/
	constexpr int WARNING_TIME = 1;

	/*UŒ‚ŠÔŠu(•b)*/
	constexpr int ATTACK_TIME = 1;

	/*–³“GŠÔ(•b)*/
	constexpr int INVINCIBLE_TIME = 5;

	/*ˆÚ“®‘¬“x*/
	constexpr float MOVE_SPEED = 0.2f;

	/*õ“G”ÍˆÍ‚ÌŠp“x*/
	constexpr float FAN_RANGE = 180.0f;

	/*õ“G”ÍˆÍ‚Ì‹——£*/
	constexpr float FAN_LENGTH = 50.0f;

	/*”¼Œa*/
	constexpr float RADIUS = 2.0f;

	/*UŒ‚”ÍˆÍ*/
	constexpr float ATTACK_LENGTH = 30.0f;

	/*ñU‚èŠp“x*/
	constexpr float SWING_RANGE = 90.0f;

	/*‘å‚«‚³*/
	const Vector3 SCALE = Vector3(2.5f, 2.5f, 2.5f);

	/*–Cg‚Ìƒ‚ƒfƒ‹–¼*/
	const string TANK_BARREL_NAME = "SniperBarrel";

	/*–C“ƒ‚Ìƒ‚ƒfƒ‹–¼*/
	const string TANK_TURRET_NAME = "SniperTurret";

	/*Ô‘Ì‚Ìƒ‚ƒfƒ‹–¼*/
	const string TANK_BODY_NAME = "SniperBody";
}