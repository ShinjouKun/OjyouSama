#pragma once

#include "../../Math/Math.h"

//基本的な設定
struct ParticleSystems
{
	float Duration = 1.0f;//パーティクルを放出する時間の長さ。ループの場合、1サイクルの長さ
	bool Looping = false;//ループするかどうか
	float StartDelay = 0.0f;//開始するまでの時間
	float StartLifeTime = 5.0f;//生存時間
	float StartSpeed = 1.0f;//最初のスピード
	Vector3 StartVector = Vector3::zero;//方向
	Vector3 StartSize3D = Vector3::one;//最初の大きさ
	Vector3 StartRotation3D = Vector3::zero;//最初の回転
	Vector4 StartColor = Vector4::one;//開始時の色
	float GravityModifier = 0.0f;//重力
};

//1度に出す量
struct Burst
{
	float Time = 1.f;//全て出すまでの時間
	int Count = 1000;//1フレームに出す量
	int Cycles = 1;//
	float Interval = 0.01f;//インターバル
	float Probabillity = 1.f;//
};

//エミッターからパーティクルに対する変化量(1フレーム毎にこの値が追加されていく)
struct AmountOfChangeInParticles
{
	float speed = 0.0f;//スピード
	Vector3 vector = Vector3::zero;//方向の変化
	Vector3 size = Vector3::zero;//サイズ
	Vector3 rotate = Vector3::zero;//回転
	Vector4 color = Vector4::zero;//色
};

//パーティクルのランダム性(パーティクルシステムに+して0～の値が入る)
struct ParticleRandomState
{
	float randomSpeed = 0.0f;//ランダムなスピードを追加
	float randomLife = 0.0f;//ランダムな生存時間を追加
	Vector3 randomSize3D = Vector3::zero;//ランダムなサイズを追加
	Vector3 randomRotate3D = Vector3::zero;//ランダムな回転を追加
	Vector4 randomColor = Vector4::zero;//ランダムなカラーを追加
};

//パーティクルの回転運動用
struct ParticleRotateState
{
	float angle = 0.0f;
	Vector3 Axis = Vector3::zero;
};