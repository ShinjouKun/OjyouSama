#pragma once
#include "Weapon.h"
#include"../ParticleSystem/ParticleType/Hit.h"
class NormalBullet:public Weapon
{
public:
	NormalBullet(const Vector3& pos,const Vector3& ang, ObjectManager* obj, 
		shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, ObjectType t, int num,int upD);
	~NormalBullet();

private:


	// Weapon を介して継承されました
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

private:
	ObjectManager* objM;
	shared_ptr<Hit> mDamageParticle;
	shared_ptr<ModelRenderer>BulletModel;
	shared_ptr<ParticleManager>BulletParticle;
	shared_ptr<ParticleEmitterBox>BulletParticleBox;

};
