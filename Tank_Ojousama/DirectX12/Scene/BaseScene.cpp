#include "BaseScene.h"

float BaseScene::mMasterSoundVol = 0.5f;
float BaseScene::mBGMSoundVol =  0.5f;
float BaseScene::mSESoundVol =  0.8f;
int BaseScene::mMoney = 15000000;
int BaseScene::mPlusMoney = 0;
int BaseScene::mMinusMoney = 0;
bool BaseScene::mStageFlag1 = false;
bool BaseScene::mStageFlag2 = false;
bool BaseScene::mStageFlag3 = false;
Vector3 BaseScene::OpAim1 = Vector3((Window::Window_Width / 2) - 32, 180, 0);
Vector3 BaseScene::OpAim2 = Vector3((Window::Window_Width / 2) - 32, 360, 0);
Vector3 BaseScene::OpAim3 = Vector3((Window::Window_Width / 2) +64, 540, 0);
Vector3 BaseScene::OpAimA1 = Vector3((Window::Window_Width / 2) - 32, 180, 0);
Vector3 BaseScene::OpAimA2 = Vector3((Window::Window_Width / 2) - 32, 360, 0);
Vector3 BaseScene::OpAimA3 = Vector3((Window::Window_Width / 2) + 64, 540, 0);

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

void BaseScene::Finalize()
{
	FinalizeScene();
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
