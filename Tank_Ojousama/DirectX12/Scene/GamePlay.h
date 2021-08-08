#pragma once
#include<memory>
#include"BaseScene.h"
#include "../Device/Input.h"
#include "../Actor/Player.h"
#include "../Actor/PlayerUI.h"
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


	// BaseScene ����Čp������܂���
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	ObjectManager* objM;
	Player* player;//�v���C���[�̎��̐���
	PlayerUI* playerUI;
	EnemyTank* enemyTank;
	T_REX* rex;
	TaihouEnemy* taihou;
	Camera* camera;
	float x, y, z;
};
