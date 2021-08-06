#pragma once
#include<memory>
#include"BaseScene.h"
//#include"Input.h"
//#include"Player.h"
//#include"EnemyTank.h"
//#include"Block.h"
//#include"T_REX.h"
//#include"TaihouEnemy.h"
//#include"ObjectManager.h"
//#include"Camera.h"
//#include"ModelRenderer.h"
//#include"TexRenderer.h"
//#include"ParticleManager.h"
#include "../Device/Input.h"
#include "../Actor/Player.h"
#include "../Actor/EnemyTank.h"
#include "../Actor/Block.h"
#include "../Actor/T_REX.h"
#include "../Actor/TaihouEnemy.h"
#include "../Actor/ObjectManager.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Render/ParticleManager.h"
class GamePlay:public BaseScene
{
public:
	GamePlay();
	~GamePlay();

private:


	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	ObjectManager* objM;
	Player* player;//ƒvƒŒƒCƒ„[‚ÌÀ‘Ì¶¬
	EnemyTank* enemyTank;
	T_REX* rex;
	TaihouEnemy* taihou;
	Camera* camera;
	float x, y, z;
};
