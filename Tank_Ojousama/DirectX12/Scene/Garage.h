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
#include "../Utility/ModelChanger.h"
class Sound;
class Timer;
class Garage :public BaseScene
{
public:
	Garage();
	~Garage();

private:

	// BaseScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	ObjectManager* objM;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mSE;
	std::shared_ptr<Sound> mBGM;

	float fade;
	bool fadeF;
	bool ChangeFlag;
	int keyflag;
	float SAlfa1;
	float SAlfa2;
	float SAlfa3;
	float SAlfa4;
	bool fadeFB;
	int headNum;
	int bodyNum;
	int bottomNum;
	int weaponNum;
	float gSelect;
	int Money = 0;
	bool buy = false;
	bool buy2 = false;
	bool buy3 = false;
	bool buy4 = false;
	Camera* camera;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 SentakuPos1;
	Vector3 SentakuPos2;
	Vector3 SentakuPos3;
	Vector3 SentakuPos4;
	Vector3 SentakuPos5;
	Vector3 SentakuPos6;
	Vector3 SentakuPos7;
	Vector3 SentakuPos8;
	Vector3 Syata;
	std::shared_ptr<Timer> mTimer;

	ModelChanger* mChanger;
};
