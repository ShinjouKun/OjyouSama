#pragma once
#include"Weapon.h"

class Explosion;

class LandMine :public Weapon
{
public:
	LandMine(const Vector3& pos, const Vector3& ang, ObjectManager* obj,
		shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, ObjectType t, int num,int upD);
	~LandMine();
private:
	// Weapon を介して継承されました
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
	shared_ptr<Explosion> mParticle;
	float bomSpace;
	bool bomFlag;
};