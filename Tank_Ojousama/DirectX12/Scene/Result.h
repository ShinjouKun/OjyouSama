#pragma once
#include<memory>
#include"BaseScene.h"
#include "../Device/Input.h"
#include "../Actor/Player.h"
#include "../Actor/ObjectManager.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"

class Sound;
class Timer;
class Result :public BaseScene
{
public:
	Result();
	~Result();

private:
	// BaseScene ����Čp������܂���
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	virtual void FinalizeScene() override;

	ObjectManager* objM;
	shared_ptr<ModelRenderer>playerModel;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mFanfare;

	std::shared_ptr<Sound> donSE;
	std::shared_ptr<Sound> dodonSE;
	std::shared_ptr<Sound> minusSE;
	std::shared_ptr<Sound> plusSE;
	Player* player;//�v���C���[�̎��̐���
	Camera * camera;
	int time = 0;

	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	float speed;
	float ojyouZ;
	float ojyouY;
	float ojyouXR;
	float ojyouXL;
	float angle;
	float zensin;
	int mMoney_PM;
	int moneyTime;
	std::shared_ptr<Timer> mTimer;

	int CountUpMoney_Get;
	int CountUpMoney_Bullet;
	int CountUpMoney_Goukei;
	int CountUpMoney_Gunshikin;
	

	bool C_Get;
	bool C_Bullet;
	bool C_Goukei;
	bool C_Gunshikin;

	int RESULT_NUM_TIME = 50;
};