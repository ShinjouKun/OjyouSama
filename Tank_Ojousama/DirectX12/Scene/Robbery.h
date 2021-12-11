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

private:

	Camera* mCamera;           //�J����
	ItemHolder* mItemHolder;   //�A�C�e���z���_�[
	ObjectManager* mObjManager;//�I�u�W�F�N�g�}�l�[�W���[

	std::shared_ptr<Sound> mBGM;  //BGM�Đ�
	std::shared_ptr<Sound> mSE;	  //���|�[�Y�J������
	std::shared_ptr<Timer> mTimer;//�|�[�Y�p�^�C�}�[
	std::shared_ptr<BreadCrumbCreater>mBreadCreator;//�p����������
	//std::shared_ptr<WayPointManager> mpointManager; //WayPoint����
	//std::shared_ptr<EnemyAI> mEnemyAI;              //�GAI����

	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 optionPos;

	int objectCount;//�I�u�W�F�N�g��
	int mTime = 0;

	//�Q�[���i�s�Ǘ��p
	bool mTreasureGet = false;   //�ڕW����͂��Ă��邩�H
	bool mGoalFlag = false;      //�S�[���ɂ��邩�H
	float mGoalLine;             //�S�[����Z�n�_
	bool mStartSpownFlag = false;//�ŏ����琶������Ă���G
	bool mBackSpownFlag = false; //�󔠂��������o������G
	bool mPose = false;          //�|�[�Y�t���O
	bool mSettingFlag = false;   //�ݒ�t���O
	bool mResultFlag = false;    //���U���g�t���O
	bool OpFlag1;                //�{�����[���p�t���O01
	bool OpFlag2;                //�{�����[���p�t���O02
	bool OpFlag3;                //�{�����[���p�t���O03
};