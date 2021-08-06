#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include<memory>
//#include"ModelRenderer.h"
//#include"ParticleManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
class TaihouEnemy:public BaseObject
{
public:
	TaihouEnemy(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p,int num);
	~TaihouEnemy();
	void Shot();
	void Move();
private:


	// BaseObject ����Čp������܂���
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	ObjectManager* objM;
	shared_ptr<ModelRenderer>taihouModel;
	shared_ptr<ParticleManager>taihouParticle;
	shared_ptr<ParticleEmitterBox>taihouParticleBox;
	int HP;
	bool ShotFlag;
	int ShotCount;
	bool HitFlag;//���G���Ԃ̂���
	int HitCount;
	int bulletStock;//�e�������ς���������

	//���������p
	int number = 0;
	string name;//�L�[���[�h�o�^
	string name2;
	string num;//�����p�i���o�[
	string numName;//���̕�����
	string numName2;//���̕�����
	int Count;

	// BaseObject ����Čp������܂���
	virtual void OnCollison(const CollisonInfo & info) override;

	// BaseObject ����Čp������܂���
	virtual void ImGuiDebug() override;
};
