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
class Title:public BaseScene
{
public:
	Title();
	~Title();
	
private:

	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;	
	ObjectManager* objM;
	Player* player;//ƒvƒŒƒCƒ„[‚ÌÀ‘Ì¶¬
	
};
