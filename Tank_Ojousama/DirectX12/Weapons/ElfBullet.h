#pragma once
#include "Weapon.h"

//’Êí’e(–î‚Ìƒ‚ƒfƒ‹ver.)ƒNƒ‰ƒX
class ElfBullet : public Weapon
{
public:

	ElfBullet(
		const Vector3& pos,
		const Vector3& ang,
		ObjectManager* objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<ParticleManager>particleManager,
		ObjectType objectType,
		int num
	);

	~ElfBullet();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

private:

	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mParticleManager;
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	const int ALIVE_COUNT = 50;

	const float mRotValue = 20.0f;
	float rot = 0.0f;
};