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
class BlackSmoke;

class GameOver :public BaseScene
{
public:
	GameOver();
	~GameOver();

private:
	// BaseScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	ObjectManager* objM;
	shared_ptr<ModelRenderer>playerModel;
	std::shared_ptr<Sound> mSound;
	Player* player;//ÉvÉåÉCÉÑÅ[ÇÃé¿ëÃê∂ê¨
	Camera * camera;
	int time = 0;

	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	int speed;
	float ojyouZ;
	float ojyouY;
	float ojyouXR;
	float ojyouXL;
	float pos = 3;

	std::shared_ptr<Timer> mTimer;
	std::shared_ptr<Timer> mParticleTimer;
	std::shared_ptr<ParticleEmitterBox>ParticleBox;
	std::shared_ptr<BlackSmoke> mHit;
};