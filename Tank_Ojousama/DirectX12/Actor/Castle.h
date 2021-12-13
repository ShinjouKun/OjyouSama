#pragma once
#include"BaseObject.h"
#include"ObjectManager.h"
#include"../Render/ParticleManager.h"
#include"../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"

class Sound;
class Explosion;

class Castle:public BaseObject
{
public:
	Castle(Vector3 pos,Vector3 angle,ObjectManager* objM,
		shared_ptr<ModelRenderer>modelRender, shared_ptr<TexRenderer>s, shared_ptr<ParticleManager> effect);
	~Castle();

private:


	// BaseObject ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;

	void translate(const Vector3& pos);
private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>Model;
	shared_ptr<TexRenderer>Sprite;
	shared_ptr<ParticleManager>Particle;
	shared_ptr<ParticleEmitterBox>ParticleBox;

	shared_ptr<Sound> mSound;
	shared_ptr<Explosion> mExplosion1;
	shared_ptr<Explosion> mExplosion2;
	shared_ptr<Explosion> mExplosion3;
	shared_ptr<Explosion> mExplosion4;
	shared_ptr<Explosion> mExplosion5;

	float mRange;

	//ñ≥ìGéûä‘ÇÃÇΩÇﬂ
	bool HitFlag;
	int HitCount;
	int MaxHp;
	int MessCount1;
	int MessCount2;
	int MessCount3;

	bool MessFlag1;
	bool MessFlag2;
	bool MessFlag3;
};
