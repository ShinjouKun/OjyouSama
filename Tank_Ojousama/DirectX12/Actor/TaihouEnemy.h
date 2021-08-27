#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include<memory>
//#include"ModelRenderer.h"
//#include"ParticleManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
class TaihouEnemy:public BaseObject
{
public:
	TaihouEnemy(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p,int num);
	~TaihouEnemy();
	void Shot();
	void Move();
private:


	// BaseObject を介して継承されました
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	ObjectManager* objM;
	shared_ptr<ModelRenderer>taihouModel;
	shared_ptr<ParticleManager>taihouParticle;
	shared_ptr<ParticleEmitterBox>taihouParticleBox;
	int HP;
	bool ShotFlag;
	int ShotCount;
	bool HitFlag;//無敵時間のため
	int HitCount;
	int bulletStock;//弾をいっぱいだすため

	//複数だす用
	int number = 0;
	string name;//キーワード登録
	string name2;
	string num;//複数用ナンバー
	string numName;//上二つの複合体
	string numName2;//上二つの複合体
	int Count;

	// BaseObject を介して継承されました
	virtual void OnCollison(BaseCollider* col) override;

	// BaseObject を介して継承されました
	virtual void ImGuiDebug() override;
};
