#pragma once
#include<memory>
#include"BaseScene.h"
#include"Input.h"
#include"Player.h"
#include"EnemyTank.h"
#include"Block.h"
#include"T_REX.h"
#include"TaihouEnemy.h"
#include"ObjectManager.h"
#include"Camera.h"
#include"ModelRenderer.h"
#include"TexRenderer.h"
#include"ParticleManager.h"
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
	T_REX* rex;
	TaihouEnemy* taihou;
	Camera* camera;
	float x, y, z;
};
