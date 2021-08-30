#pragma once
#include "../Actor/ObjectManager.h"
#include"../Actor/BaseObject.h"
#include"../Render/ModelRenderer.h"
#include"../Render/ParticleManager.h"
#include<memory>
//こいつは武器の基底クラスとする

class  Weapon:public BaseObject
{
public:
	 Weapon();
	 virtual ~Weapon();
	
	//誰の弾か
	ObjectType setType;
	void SetBulletType();
protected:
	float speed;
	int alive;//弾の生存時間
	//複数だす用
	int number = 0;
	string name;//キーワード登録
	string num;//複数用ナンバー
	string numName;//上二つの複合体
	// BaseObject を介して継承されました
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Rend() = 0;
	virtual void ImGuiDebug() = 0;
	virtual void OnCollison(BaseCollider * col) = 0;
};
