#pragma once
#include"BaseScene.h"
#include<memory>
#include "../Device/Input.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Collision/CollisionPrimitive.h"
#include "../Actor/ObjectManager.h"
#include "GamePlay.h"

class Timer;
class Sound;
class Select :public BaseScene
{
public:
	Select();
	~Select();

private:

	// BaseScene ����Čp������܂���
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;
	ObjectManager* objM;
	//GamePlay* g;

	Vector3 position;
	Vector3 selectposition;

	bool fadeF1;
	bool fadeF2;
	bool fadeF3;
	bool fadeF4;
	float fade;
	bool selectFlag = false;
	//
	bool cameramoveFlag = false;
	float SelectAlfa1;
	float SelectAlfa2;
	float SelectAlfa3;
	float SelectAlfa4;
	float SelectAlfa5;
	float angle;
	//�����p�t���O
	bool setumeiFlag = false;
	bool setumeiBossFlag = false;
	//�^�[�Q�b�g�P
	Vector3 targetPos1;
	//�^�[�Q�b�g�Q
	Vector3 targetPos2;
	//�^�[�Q�b�g�R
	Vector3 targetPos3;
	//�^�[�Q�b�g�S
	Vector3 targetPos4;
	//�I�v�V����
	Vector3 option;
	//�^�C�g���ɖ߂�
	Vector3 titleback;
	//�K���[�W
	Vector3 garege;
	//�o��
	Vector3 syutu;
	//�������
	Vector3 operation;
	//�����p
	Vector3 setumei;
	//�J�����|�W�V����
	Vector3 camerapos;
	//�^�[�Q�b�g�p�|�W�V����
	Vector3 setcamerapos;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mSE;
	std::shared_ptr<Sound> mDecisionSE;

	std::shared_ptr<Timer> mTimer;
};
