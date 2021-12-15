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
#include "../Items/Repair.h"

#include "../Actor/Enemy/EnemyAI.h"

class Sound; 
class Timer;
class WayPointManager;
class BreadCrumbCreater;

class Defense :public BaseScene
{
public:
	Defense();
	~Defense();

private:

	void Wave1EnemySpown();
	void Wave2EnemySpown();
	void Wave3EnemySpown();
	void Wave1();
	void Wave2();
	void Wave3();
	
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
	std::shared_ptr<Sound> mSE;
	//�E�F�[�u����p
	Vector3 waveMove;
	int interval = 0;//���̃E�F�[�u�܂ł̑ҋ@�J�E���g
	int spownCount;
	
	bool spown1;//�G�l�~�[�o���p�̃t���O
	bool spown2;//�G�l�~�[�o���p�̃t���O
	bool spown3;//�G�l�~�[�o���p�̃t���O
	int enemyDeath;
	int wave1EnemysCount;
	int wave2EnemysCount;
	int wave3EnemysCount;
	bool wave1Clear;
	bool wave2Clear;
	bool wave3Clear;
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
	//������t���O
	bool mCastleCollapses;

	//�o��^�C�~���O
	bool first;
	bool second;
	bool third;
	std::shared_ptr<Timer> mEnemySpownTimer;
	int enemyCount;
	bool check;
	
	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 optionPos;

	ItemHolder* itemHolder;

	shared_ptr<BreadCrumbCreater>mBreadCreator;
	shared_ptr<WayPointManager> mpointManager;

	std::shared_ptr<Timer> mTimer;
	shared_ptr<EnemyAI> mEnemyAI;
	std::shared_ptr<ParticleEmitterBox>ParticleBox;
};