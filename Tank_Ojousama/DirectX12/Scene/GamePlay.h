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
#include "../Render/ParticleManager.h"

#include "../Actor/SniperEnemy.h"
#include "../Actor/BlowEnemy.h"
#include"../Actor/GolemEnemy.h"

class GamePlay:public BaseScene
{
public:
	GamePlay();
	~GamePlay();

private:


	// BaseScene を介して継承されました
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	ObjectManager* objM;
	Player* player;//プレイヤーの実体生成
	EnemyTank* enemyTank;
	Camera* camera;
	float x, y, z;
};
