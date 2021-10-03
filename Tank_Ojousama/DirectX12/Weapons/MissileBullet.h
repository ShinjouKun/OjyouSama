#pragma once
#include"Weapon.h"

class MissileBullet:public Weapon
{
public:
	MissileBullet(const Vector3& pos, const Vector3& ang, ObjectManager* obj,
		shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, ObjectType t, int num);
	~MissileBullet();


private:

	// Weapon ����Čp������܂���
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

	void LookOn();//�^�[�Q�b�g�̃��b�N
	Vector3 Look();
private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>Model;
	shared_ptr<ParticleManager>Particle;
	shared_ptr<ParticleEmitterBox>ParticleBox;
	int attackStart;
	bool targetSet = false;
	BaseObject* obj = nullptr;
};
