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
class Timer;

class Operation :public BaseScene
{
public:
	Operation();
	~Operation();

private:

	// BaseScene を介して継承されました
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	virtual void FinalizeScene() override;

	Camera* camera;
	ObjectManager* objM;
	std::shared_ptr<Sound> mBGM;
	std::shared_ptr<Sound> mSE;
	std::shared_ptr<Timer> mTimer;
};