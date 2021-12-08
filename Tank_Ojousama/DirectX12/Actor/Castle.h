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


	// BaseObject ����Čp������܂���
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

	//���G���Ԃ̂���
	bool HitFlag;
	int HitCount;
};
