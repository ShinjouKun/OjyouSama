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

class Sound;
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
	//�|�[�Y
	void Pose();
	//���ʐݒ�
	void Setting();

	ObjectManager* objM;
	Player* player;//�v���C���[�̎��̐���
	EnemyTank* enemyTank;
	Camera* camera;
	std::shared_ptr<Sound> mSound;
	float x, y, z;

	//�|�[�Y�t���O
	bool pose = false;
	//�ݒ�t���O
	bool settingFlag = false;
	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;;

};
