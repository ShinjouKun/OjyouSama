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
class Select :public BaseScene
{
public:
	Select();
	~Select();

private:

	// BaseScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;
	ObjectManager* objM;
	//GamePlay* g;

	Vector3 position;
	Vector3 selectposition;

	float b;
	bool selectFlag = false;
	Vector3 targetPos1;
	Vector3 targetPos2;
	Vector3 targetPos3;
	Vector3 targetPos4;

};
