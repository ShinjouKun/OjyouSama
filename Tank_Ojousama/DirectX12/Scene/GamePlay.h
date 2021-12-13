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
#include"../Actor/GolemEnemy.h"
#include "../Items/ItemHolder.h"

#include "../Actor/Enemy/EnemyAI.h"

class SniperEnemy;
class BlowEnemy;
class WayPointManager;
class BreadCrumbCreater;
class Sound;
class Timer;
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
	//���U���g���
	void ResultF();

	ObjectManager* objM;
	Player* player;//�v���C���[�̎��̐���
	EnemyTank* enemyTank;
	Camera* camera;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mHidan;
	float x, y, z;

	ItemHolder* itemHolder;

	//�|�[�Y�t���O
	bool pose = false;
	//�ݒ�t���O
	bool settingFlag = false;
	//���U���g�t���O
	bool resultFlag = false;
	int time = 0;
	float timer = 0;
	//�{�����[���p�t���O
	bool OpFlag1;
	bool OpFlag2;
	bool OpFlag3;

	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 optionPos;
	//BreadCrumbCreater* mBreadCreator;
	//WayPointManager * mpointManager;

	shared_ptr<BreadCrumbCreater>mBreadCreator;
	shared_ptr<WayPointManager> mpointManager;

	shared_ptr<EnemyAI> mEnemyAI;
	std::shared_ptr<Timer> mTimer;

	std::shared_ptr<Timer> mParticleTimer;
	std::shared_ptr<ParticleEmitterBox>ParticleBox;

	Camera* cam;
	int goleamDeath;
	bool spawnFlag;//��Ԗ�
	bool spawnFlag2;//���
	bool spawnFlag3;//�O��
};
