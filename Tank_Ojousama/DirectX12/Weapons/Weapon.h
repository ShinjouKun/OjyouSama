#pragma once
#include "../Actor/ObjectManager.h"
#include"../Actor/BaseObject.h"
#include"../Render/ModelRenderer.h"
#include"../Render/ParticleManager.h"
#include<memory>
//�����͕���̊��N���X�Ƃ���

class  Weapon:public BaseObject
{
public:
	 Weapon();
	 virtual ~Weapon();
	
	//�N�̒e��
	ObjectType setType;
	void SetBulletType();
protected:
	float speed;
	int alive;//�e�̐�������
	//���������p
	int number = 0;
	string name;//�L�[���[�h�o�^
	string num;//�����p�i���o�[
	string numName;//���̕�����
	// BaseObject ����Čp������܂���
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Rend() = 0;
	virtual void ImGuiDebug() = 0;
	virtual void OnCollison(BaseCollider * col) = 0;
};
