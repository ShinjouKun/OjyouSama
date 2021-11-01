#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"

//�J�����ɓ����蔻��Ȃǌ��݂�static�ɕt�����炢�A��ɂ͕K�v�Ȃ��@�\����t������
class CameraEye:public BaseObject
{
public:
	CameraEye(Vector3 pos,Vector3 angle, ObjectManager * obj);
	~CameraEye();

private:


	// BaseObject ����Čp������܂���
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;
private:
	ObjectManager* objM;
};
