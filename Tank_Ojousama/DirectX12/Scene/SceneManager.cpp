#include "SceneManager.h"
#include"Title.h"
#include"GamePlay.h"
#include "Result.h"
#include "../ParticleSystem/ParticleSystem.h"
#include "GameOver.h"

SceneManager::SceneManager(shared_ptr<TexRenderer> sprite, shared_ptr<ModelRenderer> model, shared_ptr<ParticleManager>particle)
	:mSprite(sprite), mModel(model), mParticle(particle),
mNowScene(std::make_shared<Title>())//最初はタイトル
{
	SetSpriteToScene();
	SetModelToScene();
	SetParticleToScene();
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	auto next = mNowScene->GetNextScene();
	if (next)
	{
		mNowScene->Finalize();
		//パーティクルシステムの中身リセット
		ParticleSystem::instance().reset();
		
		mNowScene = next;
		
		SetModelToScene();
		SetParticleToScene();
		SetSpriteToScene();
	}
	//順番に注意
	mNowScene->Update();
	//mParticle->OllUpDate();
}

void SceneManager::Draw() const
{
	mNowScene->Draw();
	//mParticle->OllDraw();
}

void SceneManager::Finalize()
{
	mNowScene->Finalize();
}

void SceneManager::SetSpriteToScene()
{
	mNowScene->SetSprite(mSprite);
}

void SceneManager::SetModelToScene()
{
	mNowScene->SetModel(mModel);
}

void SceneManager::SetParticleToScene()
{
	mNowScene->SetParticle(mParticle);
}
