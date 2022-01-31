#pragma once
#include<memory>
#include"BaseScene.h"
#include "../Device/Input.h"
#include "../Actor/ObjectManager.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"

class Sound;
class Timer;
class EndRoll :public BaseScene
{
public:
	EndRoll();
	~EndRoll();

private:
	// BaseScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	virtual void FinalizeScene() override;

	ObjectManager* objM;
	std::shared_ptr<Sound> mSound;
	Camera * camera;
	int time = 0;

	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	int speed;
	float fade1;
	float fade2;
	bool fadeF;

	std::shared_ptr<Timer> mTimer;
	std::shared_ptr<Timer> mParticleTimer;
	std::shared_ptr<ParticleEmitterBox>ParticleBox;
};