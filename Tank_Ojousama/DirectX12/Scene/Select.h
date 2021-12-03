#pragma once
#include"BaseScene.h"
#include<memory>
#include "../Device/Input.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Collision/CollisionPrimitive.h"
#include "../Actor/ObjectManager.h"
#include "GamePlay.h"

class Timer;
class Sound;
class Select :public BaseScene
{
public:
	Select();
	~Select();

private:

	// BaseScene を介して継承されました
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;
	ObjectManager* objM;
	//GamePlay* g;

	Vector3 position;
	Vector3 selectposition;

	bool fadeF1;
	bool fadeF2;
	bool fadeF3;
	bool fadeF4;
	float fade;
	bool selectFlag = false;
	//
	bool cameramoveFlag = false;
	float SelectAlfa1;
	float SelectAlfa2;
	float SelectAlfa3;
	float SelectAlfa4;
	float SelectAlfa5;
	float angle;
	//説明用フラグ
	bool setumeiFlag = false;
	bool setumeiBossFlag = false;
	//ターゲット１
	Vector3 targetPos1;
	//ターゲット２
	Vector3 targetPos2;
	//ターゲット３
	Vector3 targetPos3;
	//ターゲット４
	Vector3 targetPos4;
	//オプション
	Vector3 option;
	//タイトルに戻る
	Vector3 titleback;
	//ガレージ
	Vector3 garege;
	//出撃
	Vector3 syutu;
	//操作説明
	Vector3 operation;
	//説明用
	Vector3 setumei;
	//カメラポジション
	Vector3 camerapos;
	//ターゲット用ポジション
	Vector3 setcamerapos;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mSE;
	std::shared_ptr<Sound> mDecisionSE;

	std::shared_ptr<Timer> mTimer;
};
