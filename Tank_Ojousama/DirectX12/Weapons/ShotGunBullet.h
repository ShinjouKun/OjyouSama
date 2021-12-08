#pragma once
#include"Weapon.h"
//�V���b�g�K���Ƃ������U�e�̂P�y���b�g�Ƃ��ẴN���X
class ShotGunBullet :public Weapon
{
public:
	ShotGunBullet(const Vector3& pos, const Vector3& ang, ObjectManager* obj,
		shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, ObjectType t, int num,int upD);
	~ShotGunBullet();

private:
	// Weapon ����Čp������܂���
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