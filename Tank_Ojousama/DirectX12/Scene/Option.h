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
class Option :public BaseScene
{
public:
	Option();
	~Option();

private:

	// BaseScene を介して継承されました
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	virtual void FinalizeScene() override;

	Camera* camera;
	ObjectManager* objM;
	Player* player;//プレイヤーの実体生成

	//ボリューム用フラグ
	bool OpFlag1;
	bool OpFlag2;
	bool OpFlag3;

	Vector3 posePos;
	Vector3 optionPos;

	std::shared_ptr<Sound> mSound;

	std::shared_ptr<Timer> mTimer;
};
