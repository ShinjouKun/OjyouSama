#pragma once
#include<memory>
//#include"TexRenderer.h"//スプライト用
//#include"ModelRenderer.h"//モデル用
//#include"ParticleManager.h"//パーティクル
//#include"DirectXManager.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include "../Device/DirectXManager.h"
using namespace std;
class BaseScene
{
protected:
	BaseScene();
	virtual~BaseScene();
	shared_ptr<TexRenderer>mSprite;
	shared_ptr<ModelRenderer>mModel;
	shared_ptr<ParticleManager>mParticle;
	
public:
	virtual void StartScene() = 0;
	void Update();
	virtual void UpdateScene() = 0;
	void Draw();
	virtual void DrawScene() = 0;
	void NextScene(shared_ptr<BaseScene>next);
	std::shared_ptr<BaseScene>GetNextScene()const;
	void SetSprite(shared_ptr<TexRenderer>sprite);
	void SetModel(shared_ptr<ModelRenderer>model);
	void SetParticle(shared_ptr<ParticleManager>particle);
private:
	std::shared_ptr<BaseScene>mNext;
	void Start();
	bool isInit;//初期化
};
