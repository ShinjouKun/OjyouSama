#pragma once
#include<memory>
#include"BaseScene.h"
//#include"TexRenderer.h"
//#include"ModelRenderer.h"
//#include"ParticleManager.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

using namespace std;
class SceneManager
{
public:
	SceneManager(shared_ptr<TexRenderer> sprite,
	shared_ptr<ModelRenderer>model, shared_ptr<ParticleManager>particle);
	~SceneManager();
	void Update();
	void Draw()const;
private:
	void SetSpriteToScene();
	void SetModelToScene();
	void SetParticleToScene();
	shared_ptr<TexRenderer>mSprite;
	shared_ptr<ModelRenderer>mModel;
	shared_ptr<ParticleManager>mParticle;
	shared_ptr<BaseScene>mNowScene;
};
