#pragma once
#include"BaseScene.h"
//#include"Input.h"
#include<memory>
//#include"Camera.h"
//#include"CollisionPrimitive.h"
//#include"ModelRenderer.h"
//#include"TexRenderer.h"
//#include"Player.h"
//#include"ObjectManager.h"
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

	// BaseScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;	
	ObjectManager* objM;
	std::shared_ptr<Sound> mSound;

	float fade1;
	float fade2;
	bool fadeF;
	Vector3 ojoP;

	float ojyouY;
	float ojyouXR;
	float ojyouXL;
	
};
