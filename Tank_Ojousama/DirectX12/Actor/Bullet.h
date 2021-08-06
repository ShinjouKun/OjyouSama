#pragma once
//#include"ObjectManager.h"
//#include"BaseObject.h"
//#include<memory>
//#include"ModelRenderer.h"
//#include"ParticleManager.h"

#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include <memory>

class Bullet:public BaseObject
{
public:
	Bullet(Vector3 pos, Vector3 angle, ObjectManager* obj,shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p,ObjectType t,int num);
	~Bullet();

private:
	ObjectManager* objM;
	std::shared_ptr<ModelRenderer>BulletModel;
	shared_ptr<ParticleManager>BulletParticle;
	shared_ptr<ParticleEmitterBox>BulletParticleBox;
	Vector3 TargetPos;
	int alive;
	float speed;

	//複数だす用
	int number = 0;
	string name;//キーワード登録
	string num;//複数用ナンバー
	string numName;//上二つの複合体
	// BaseObject を介して継承されました
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	
	//誰の弾か
	ObjectType setType;
	void SetBulletType();

	// BaseObject を介して継承されました
	virtual void OnCollison(const CollisonInfo & info) override;

	// BaseObject を介して継承されました
	virtual void ImGuiDebug() override;
};
