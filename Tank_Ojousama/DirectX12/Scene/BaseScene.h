#pragma once
#include<memory>
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
	void Finalize();
	virtual void FinalizeScene() = 0;
	void NextScene(shared_ptr<BaseScene>next);
	std::shared_ptr<BaseScene>GetNextScene()const;
	void SetSprite(shared_ptr<TexRenderer>sprite);
	void SetModel(shared_ptr<ModelRenderer>model);
	void SetParticle(shared_ptr<ParticleManager>particle);
	static float mMasterSoundVol;
	static float mBGMSoundVol;
	static float mSESoundVol;
	static int mPlusMoney;//プレイ中に増した金
	static int mMinusMoney;//プレイ中に減した金
	static int mMoney;
	static bool mStageFlag1;
	static bool mStageFlag2;
	static bool mStageFlag3;
	static Vector3 OpAim1;
	static Vector3 OpAim2;
	static Vector3 OpAim3;
	static Vector3 OpAimA1;
	static Vector3 OpAimA2;
	static Vector3 OpAimA3;

private:
	std::shared_ptr<BaseScene>mNext;
	void Start();
	bool isInit;//初期化

};
