#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include<memory>
#include "../Render/Camera.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include "../Collision/CollisonManager.h"
#include"../Utility/ModelChanger.h"
#include"../Items/ItemHolder.h"


class Listener;
class Sound;
class Timer;
class NormalAttackParticle;
class MachineGunAttackParticle;
class TankTrajectory;
class Hit;

using namespace std;
class Player :public BaseObject
{
public:

	Player(Vector3 pos, Vector3 angle, ObjectManager* obj, shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, int sceneEffect);
	~Player();
	void StartCamScene();
	void SceneCamMove1();//�r��
	void SceneCamMove2();//���D
	void SceneCamMove3();//�h�q
	void SceneCamMove4();//�{�X

	void UseWeapon1();//�����P
	void UseWeapon2();//�����Q
	void UseULT();//�A���e�B���b�g
	void Item();//�T�|�[�g�A�C�e��
	void AngleReset();//���l�̃I�[�o�[�t���E��h������
	// BaseObject ����Čp������܂���
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	void SetHP(int value);//�̗͕ύX
	int GetHP() { return HP; }

private:
	void TrajectoryPlay();

private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>playerModel;
	shared_ptr<ParticleManager>playerParticle;
	shared_ptr<ParticleEmitterBox>playerParticleBox;
	shared_ptr<TexRenderer>playerSprite;
	shared_ptr<Listener> listener;
	Camera* camera;
	std::shared_ptr<Sound> mNormalAttackSE;
	std::shared_ptr<Sound> mDamageSE01;
	std::shared_ptr<Sound> mDamageSE02;
	std::shared_ptr<Sound> mDeathSE;
	std::shared_ptr<Timer> mTimer;
	ModelChanger* modelChanger;

	//�J�����p
	Vector3 CameraPos;
	float CamPos_Y;
	Vector3 TargetPos;
	Vector3 CamVelocity;//�J�����̈ړ���
	//����p�t���O

	bool sniperShotFlag = false;
	bool shotFlag1 = false;
	bool shotFlag2 = false;
	bool masingunShot = false;
	int shotcnt1 = 0;
	int shotcnt2 = 0;
	bool moveFlag;
	bool FrontMove;
	bool BackMove;
	Vector3 AIMPos;
	//�X�e�[�^�X
	int weaponMoveNum;
	int UpDamage;//�ǉ��U����
	ItemHolder* item;
	bool GameOver = false;
	int bulletStock;//�e�������ς���������
	float atkAngle;//�C���p//�`��
	float fireAngle;//�C�g
	float cameraSpeed;
	int speedTime;
	int maxHP;//�ō�HP
	float maxSpeed;//�ō����x
	float speedLimitTime;
	bool HitFlag;//���G���Ԃ̂���
	int HitCount;
	float ojyouY;
	float ojyouZ;
	float ojyouXR;
	float ojyouZR;
	float ojyouXL;
	float ojyouZL;
	float aimPos_Y;
	int sceneEffectNum;
	int sceneCount;
	bool sceneCamOk;
	Vector3 sceneCamPos;
	bool sceneCamPlayerOk;
	float damageFade;
	float damageFadeYpos;
	//�T�E���h

	// BaseObject ����Čp������܂���
	virtual void OnCollison(BaseCollider* col) override;

	// BaseObject ����Čp������܂���
	virtual void ImGuiDebug() override;

	//�p�[�e�B�N��
	std::shared_ptr<NormalAttackParticle> mNormalAtkParticle;//Weapon1
	std::shared_ptr<MachineGunAttackParticle> mMGAParticle;//Weapon2
	std::shared_ptr<TankTrajectory> mTankTraL;//�O��
	std::shared_ptr<TankTrajectory> mTankTraR;//�O��
	std::shared_ptr<Hit> mHit;//������
};