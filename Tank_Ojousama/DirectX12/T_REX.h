#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include<memory>
#include"ModelRenderer.h"
#include"ParticleManager.h"
class T_REX:public BaseObject
{
public:
	T_REX(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p);
	~T_REX();
	void Fire();
private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>REXModel;
	shared_ptr<ParticleManager>REXParticle;
	shared_ptr<ParticleEmitterBox>REXParticleBox;
	float KutiAngle;//���p
	float angleR;
	float angleL;

	// BaseObject ����Čp������܂���
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	// BaseObject ����Čp������܂���
	virtual void OnCollison(const CollisonInfo & info) override;


	// BaseObject ����Čp������܂���
	virtual void ImGuiDebug() override;

};
