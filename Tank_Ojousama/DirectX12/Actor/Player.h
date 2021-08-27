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
using namespace std;
class Player :public BaseObject
{
public:
	Player(Vector3 pos, Vector3 angle, ObjectManager* obj,shared_ptr<ModelRenderer>m,shared_ptr<ParticleManager>p,shared_ptr<TexRenderer>s);
	~Player();

	void Shot();
	void AngleReset();//���l�̃I�[�o�[�t���E��h������
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
	shared_ptr<Listener> listener;
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
	float cameraSpeed;
	//�T�E���h

	// BaseObject ����Čp������܂���
	virtual void OnCollison(BaseCollider* col) override;

	// BaseObject ����Čp������܂���
	virtual void ImGuiDebug() override;
	
};