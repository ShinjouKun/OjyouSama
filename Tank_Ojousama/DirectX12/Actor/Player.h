#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include<memory>
//#include"Camera.h"
//#include"TexRenderer.h"
//#include"ModelRenderer.h"
//#include"ParticleManager.h"
//#include"CollisonManager.h"
#include "../Render/Camera.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include "../Collision/CollisonManager.h"

using namespace std;
class Player :public BaseObject
{
public:
	Player(Vector3 pos, Vector3 angle, ObjectManager* obj,shared_ptr<ModelRenderer>m,shared_ptr<ParticleManager>p,shared_ptr<TexRenderer>s);
	~Player();

	void Shot();
	// BaseObject ����Čp������܂���
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
private:
	ObjectManager* objM;
    shared_ptr<ModelRenderer>playerModel;
	shared_ptr<ParticleManager>playerParticle;
	shared_ptr<ParticleEmitterBox>playerParticleBox;
	shared_ptr<TexRenderer>playerSprite;
	Camera* camera;
	//�J�����p
	Vector3 CameraPos;
	Vector3 TargetPos;
	Vector3 CamVelocity;//�J�����̈ړ���
	//����p�t���O
	bool shotFlag =false;
	int shotcnt = 0;
	bool hitFlag = false;
	bool moveFlag;
	bool FrontMove;
	bool BackMove;
	Vector3 AIMPos;
	//�X�e�[�^�X
	int HP;
	bool GameOver = false;
	int bulletStock;//�e�������ς���������
	float atkAngle;//�C���p//�`��
	float fireAngle;//�C�g
	//�T�E���h

	// BaseObject ����Čp������܂���
	virtual void OnCollison(const CollisonInfo & info) override;

	// BaseObject ����Čp������܂���
	virtual void ImGuiDebug() override;
};