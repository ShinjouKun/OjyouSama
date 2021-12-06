#pragma once
#include"BaseObject.h"
#include"ObjectManager.h"
#include"../Render/ParticleManager.h"
#include"../Render/ModelRenderer.h"

class Castle:public BaseObject
{
public:
	Castle(Vector3 pos,Vector3 angle,ObjectManager* objM,
		shared_ptr<ModelRenderer>modelRender, shared_ptr<ParticleManager> effect);
	~Castle();

private:


	// BaseObject を介して継承されました
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

	//無敵時間のため
	bool HitFlag;
	int HitCount;
};
