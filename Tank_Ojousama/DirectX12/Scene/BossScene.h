#pragma once
#include<memory>
#include"BaseScene.h"
#include "../Device/Input.h"
#include "../Actor/Player.h"
#include "../Actor/ObjectManager.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Render/ParticleManager.h"
#include "../Items/ItemHolder.h"
#include "../Items/Repair.h"
#include "../Actor/Enemy/EnemyAI.h"

class WayPointManager;
class BreadCrumbCreater;
class Sound;
class Timer;
class ElfTreeBoss;

class BossScene :public BaseScene
{
public:
	BossScene();
	~BossScene();

private:
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	virtual void FinalizeScene() override;

	/*�G�𐶐�����*/
	void EnemySpawn();

	/*�A�j���[�V�������I�����Ă��邩���Ď�*/
	void CheckAnimation();

	//�|�[�Y
	void Pose();
	//���ʐݒ�
	void Setting();
	//���U���g���
	void ResultF();

private:

	ObjectManager* mObjManager;//�I�u�W�F�N�g�}�l�[�W���[
	Player* mPlayer;           //�v���C���[�̎��̐���
	Camera* mCamera;           //�J��������
	ElfTreeBoss * mBoss;       //�{�X����
	ItemHolder* itemHolder;    //�A�C�e���z���_�[
	shared_ptr<Sound> mBGM;    //BGM����
	shared_ptr<BreadCrumbCreater>mBreadCreator;//�p����������
	shared_ptr<WayPointManager> mpointManager; //�g���Ăȁ[��
	shared_ptr<EnemyAI> mEnemyAI;              //�g���Ăȁ[��
	shared_ptr<ParticleEmitterBox>ParticleBox; //�g���Ăȁ[��
	shared_ptr<Timer> mTimer;                  //���͗p�^�C�}�[

	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 optionPos;
	
	bool pose = false;       //�|�[�Y�t���O	
	bool settingFlag = false;//�ݒ�t���O	
	bool resultFlag = false; //���U���g�t���O
	bool OpFlag1 = false;    //�{�����[���p�t���O01
	bool OpFlag2 = false;    //�{�����[���p�t���O02
	bool OpFlag3 = false;    //�{�����[���p�t���O03
	bool mBossDeadFlag;      //�{�X�����񂾂��ǂ���
	bool mCameraAnimation;   //�J�����A�j���[�V���������H
	bool mSpawn;             //�G�𐶐��������ǂ���

	float timer = 0;
	float x, y, z;

	int time = 0;
};