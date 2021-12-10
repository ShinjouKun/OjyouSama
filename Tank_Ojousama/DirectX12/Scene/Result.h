#pragma once
#include<memory>
#include"BaseScene.h"
#include "../Device/Input.h"
#include "../Actor/Player.h"
#include "../Actor/EnemyTank.h"
#include "../Actor/Block.h"
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
	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	ObjectManager* objM;
	shared_ptr<ModelRenderer>playerModel;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mFanfare;
	Player* player;//ƒvƒŒƒCƒ„[‚ÌÀ‘Ì¶¬
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

	std::shared_ptr<Timer> mTimer;
};