#include "BaseScene.h"

float BaseScene::mMasterSoundVol = 1.0f;
float BaseScene::mBGMSoundVol = 1.0f;
float BaseScene::mSESoundVol = 1.0f;
int BaseScene::mMoney = 0;

BaseScene::BaseScene()
	:mSprite(nullptr),
	mModel(nullptr),
	mParticle(nullptr),
	mNext(nullptr),
	isInit(false)
{
}

BaseScene::~BaseScene()
{
	
}

void BaseScene::Update()
{
	Start();
	UpdateScene();
}

void BaseScene::Draw()
{
	if (!mSprite)
	{
		return;
	}
	if (!mModel)
	{
		return;
	}
	if (!mParticle)
	{
		return;
	}

	DrawScene();
}

void BaseScene::NextScene(std::shared_ptr<BaseScene> next)
{
	mNext = next;
}

std::shared_ptr<BaseScene> BaseScene::GetNextScene() const
{
	return mNext;
}

void BaseScene::SetSprite(std::shared_ptr<TexRenderer> sprite)
{
	mSprite = sprite;
}

void BaseScene::SetModel(std::shared_ptr<ModelRenderer> model)
{
	mModel = model;
}

void BaseScene::SetParticle(shared_ptr<ParticleManager> particle)
{
	mParticle = particle;
}



void BaseScene::Start()
{
	if(!isInit)
	{ 
		 StartScene();
		 isInit = true;
	}
}
