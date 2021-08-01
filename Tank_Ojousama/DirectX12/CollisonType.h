#pragma once
//判定用の型定義
enum CollisonTypes
{
	UNKNOWN,//未設定、不明
	SPHERE_COLLISON,//球
	BOX_COLLISON,//箱
	PLANE_COLLISON,//平面、坂など
	RAY_COLLISON,//線
};