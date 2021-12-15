#pragma once
#include"BaseScene.h"
//#include"Input.h"
#include<memory>
#include "../Device/Input.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Collision/CollisionPrimitive.h"
#include "../Actor/Player.h"
#include "../Actor/ObjectManager.h"

class Sound;

class Title:public BaseScene
{
public:
	Title();
	~Title();
	
private:

	// BaseScene を介して継承されました
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;	
	ObjectManager* objM;
	std::shared_ptr<Sound> mBGM;
	std::shared_ptr<Sound> mSE;

	std::shared_ptr<Sound> mMoveSE; //戦車の移動SE
	std::shared_ptr<Sound> mBreakSE;//ぶつかる音
	std::shared_ptr<Timer> mIntervalTime;//タイトルロゴが出るまでの時間

	//アニメーション遷移状態
	enum AnimationStep
	{
		MOVE_FRONT,//前に向かって走ってくる
		INTERVAL,  //ぶつかったら数秒待つ
		PLAY       //タイトルロゴとかが出てくる
	}; AnimationStep mAnimStep;

	float fade1;
	float fade2;
	float kakudai;
	float kakudai2;
	float kakudai3;
	float kakudai4;
	bool fadeF;
	Vector3 ojoP;

	float ojyouY;
	float ojyouXR;
	float ojyouXL;
};
