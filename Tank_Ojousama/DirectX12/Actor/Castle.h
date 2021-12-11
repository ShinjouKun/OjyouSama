#pragma once
#include"BaseObject.h"
#include"ObjectManager.h"
#include"../Render/ParticleManager.h"
#include"../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"

class Castle:public BaseObject
{
public:
	Castle(Vector3 pos,Vector3 angle,ObjectManager* objM,
		shared_ptr<ModelRenderer>modelRender, shared_ptr<TexRenderer>s, shared_ptr<ParticleManager> effect);
	~Castle();

private:


	// BaseObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;
private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>Model;
	shared_ptr<TexRenderer>Sprite;
	shared_ptr<ParticleManager>Particle;
	shared_ptr<ParticleEmitterBox>ParticleBox;

	//–³“GŠÔ‚Ì‚½‚ß
	bool HitFlag;
	int HitCount;
	float MaxHp;
	int MessCount1;
	int MessCount2;
	int MessCount3;

	bool MessFlag1;
	bool MessFlag2;
	bool MessFlag3;
};
