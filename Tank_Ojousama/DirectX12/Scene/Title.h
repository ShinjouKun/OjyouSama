#pragma once
#include"BaseScene.h"
//#include"Input.h"
#include<memory>
#include "../Device/Input.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Collision/CollisionPrimitive.h"
#include "../Actor/Player.h"
#include "../Actor/ObjectManager.h"

class Sound;

class Title:public BaseScene
{
public:
	Title();
	~Title();
	
private:

	// BaseScene ����Čp������܂���
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;	
	ObjectManager* objM;
	std::shared_ptr<Sound> mBGM;
	std::shared_ptr<Sound> mSE;

	std::shared_ptr<Sound> mMoveSE; //��Ԃ̈ړ�SE
	std::shared_ptr<Sound> mBreakSE;//�Ԃ��鉹
	std::shared_ptr<Timer> mIntervalTime;//�^�C�g�����S���o��܂ł̎���

	//�A�j���[�V�����J�ڏ��
	enum AnimationStep
	{
		MOVE_FRONT,//�O�Ɍ������đ����Ă���
		INTERVAL,  //�Ԃ������琔�b�҂�
		PLAY       //�^�C�g�����S�Ƃ����o�Ă���
	}; AnimationStep mAnimStep;

	float fade1;
	float fade2;
	float kakudai;
	float kakudai2;
	float kakudai3;
	float kakudai4;
	bool fadeF;
	Vector3 ojoP;

	float ojyouY;
	float ojyouXR;
	float ojyouXL;
};
