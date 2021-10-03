#pragma once
#include"Weapon.h"

class MashinGun :public Weapon
{
public:
	MashinGun(const Vector3& pos, const Vector3& ang, ObjectManager* obj,
		shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, ObjectType t, int num);
	~MashinGun();
private:
	// Weapon ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;
private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>Model;
	shared_ptr<ParticleManager>Particle;
	shared_ptr<ParticleEmitterBox>ParticleBox;
};