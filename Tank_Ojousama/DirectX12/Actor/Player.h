#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include<memory>
#include "../Render/Camera.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include "../Collision/CollisonManager.h"


class Listener;
class Sound;
using namespace std;
class Player :public BaseObject
{
public:
	Player(Vector3 pos, Vector3 angle, ObjectManager* obj,shared_ptr<ModelRenderer>m,shared_ptr<ParticleManager>p,shared_ptr<TexRenderer>s);
	~Player();
	void SetCamEye();
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
	ObjectManager* objM;
    shared_ptr<ModelRenderer>playerModel;
	shared_ptr<ParticleManager>playerParticle;
	shared_ptr<ParticleEmitterBox>playerParticleBox;
	shared_ptr<TexRenderer>playerSprite;
	shared_ptr<Listener> listener;
	Camera* camera;
	std::shared_ptr<Sound> mSound;

	//�J�����p
	Vector3 CameraPos;
	float CamPos_Y;
	Vector3 TargetPos;
	Vector3 CamVelocity;//�J�����̈ړ���
	//����p�t���O
	bool sniperShotFlag = false;
	bool shotFlag1 =false;
	bool shotFlag2 = false;
	int shotcnt1 = 0;
	int shotcnt2 = 0;
	bool moveFlag;
	bool FrontMove;
	bool BackMove;
	Vector3 AIMPos;
	//�X�e�[�^�X
	//int HP;
	bool GameOver = false;
	int bulletStock;//�e�������ς���������
	float atkAngle;//�C���p//�`��
	float fireAngle;//�C�g
	float cameraSpeed;
	int speedTime;
	float maxSpeed;
	float speedLimitTime;
	bool HitFlag;//���G���Ԃ̂���
	int HitCount;
	float ojyouY;
	float ojyouXR;
	float ojyouXL;
	float aimPos_Y;
	
	
	//�T�E���h

	// BaseObject ����Čp������܂���
	virtual void OnCollison(BaseCollider* col) override;

	// BaseObject ����Čp������܂���
	virtual void ImGuiDebug() override;





	//�u1=P�L�[�ŗ��Ƃ��v�u2=�����ŗ��Ƃ��v�u3=���Ԃŗ��Ƃ��v
	void DropBreadCrumb(int status);
	//�p��������������p�}�b�v
	std::unordered_map<int, Vector3> measureMap;
	//�p�������̎��ʔԍ�
	int breadNumber = 0;
	//�p�������𗎂Ƃ��Ԋu
	int intervalCount = 0;
	int intervalTime = 1;
	
};