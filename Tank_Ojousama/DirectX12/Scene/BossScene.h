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
#include "../Items/ItemHolder.h"
#include "../Items/Repair.h"
#include "../Actor/EnemyAI.h"

class WayPointManager;
class BreadCrumbCreater;
class Sound;
class Timer;
class TestBoss;

class BossScene :public BaseScene
{
public:
	BossScene();
	~BossScene();

private:
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;
	//�|�[�Y
	void Pose();
	//���ʐݒ�
	void Setting();
	//���U���g���
	void ResultF();

	ObjectManager* mObjManager;
	Player* player;//�v���C���[�̎��̐���
	Camera* camera;
	std::shared_ptr<Sound> mSound;
	float x, y, z;
	ItemHolder* itemHolder;

	//�|�[�Y�t���O
	bool pose = false;
	//�ݒ�t���O
	bool settingFlag = false;
	//���U���g�t���O
	bool resultFlag = false;
	int time = 0;
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

	shared_ptr<BreadCrumbCreater>mBreadCreator;
	shared_ptr<WayPointManager> mpointManager;

	shared_ptr<EnemyAI> mEnemyAI;
	std::shared_ptr<Timer> mTimer;

	TestBoss * mBoss;
};