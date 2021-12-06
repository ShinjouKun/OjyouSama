#pragma once
#include "../../Math/Math.h"

//エミッターの更新に必要なデータ(パーティクル生成データ)
struct EmitterData
{
	Vector3 Pos;
	Vector3 Vec;
	float StartLifeTime;
	float StartSpeed;//最初のスピード
	Vector3 StartVector;//方向
	Vector3 StartSize3D;//最初の大きさ
	Vector3 StartRotation3D;//最初の回転
	Vector4 StartColor;//開始時の色
	float GravityModifier;//重力
	//float Time;//全て出すまでの時間
	//int Count;//1フレームに出す量
	//int Cycles;//
	//float Interval;//インターバル
	//float Probabillity;//
	float randomSpeed;//ランダムなスピードを追加
	float randomLife;//ランダムな生存時間を追加
	Vector3 randomSize3D;//ランダムなサイズを追加
	Vector3 randomRotate3D;//ランダムな回転を追加
	Vector4 randomColor;//ランダムなカラーを追加
	float Angle;
	Vector3 Axis;
};

//パーティクルの更新に必要な情報
struct ParticleData
{
	float lifeTime;//寿命
	Vector3 pos;//位置
	Vector3 vec;//ベクトル
	float temp1;//メモリ合わせ
	Vector4 col;//色
	Vector3 size;//サイズ
	float speed;//スピード
	Vector3 rotate;//回転
	Vector3 axis;//軸
	float gravity;//重力
	bool active;//生きているかどうか
};

//パーティクルの描画に必要な情報
struct ParticleDrawData
{
	Vector3 pos;
	Vector4 color;
	Vector3 size;
	Vector3 rotate;
};