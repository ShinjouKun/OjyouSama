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

class Robbery :public BaseScene
{
public:
	Robbery();
	~Robbery();

private:
	void StartWayEnemySpown();//�s����
	void BackWayEnemySpown();//�A�蓹(�A��͕����Ō����ڂł�����)


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

	Camera* camera;
	ObjectManager* objM;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mSE;
	std::shared_ptr<Timer> mTimer;
	//�Q�[���i�s�Ǘ��p
	bool treasureGet = false;//�ڕW����͂��Ă��邩�H
	bool goalFlag = false;//�S�[���ɂ��邩�H
	float goalLine;//�S�[����Z�n�_
	bool startSpownFlag = false;
	bool backSpownFlag = false;
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
	float timer = 0;
	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 optionPos;

	ItemHolder* itemHolder;

	shared_ptr<BreadCrumbCreater>mBreadCreator;
	shared_ptr<WayPointManager> mpointManager;
	shared_ptr<EnemyAI> mEnemyAI;
	std::shared_ptr<ParticleEmitterBox>ParticleBox;

};