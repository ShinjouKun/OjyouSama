#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include<memory>
//#include"Camera.h"
//#include"TexRenderer.h"
//#include"ModelRenderer.h"
//#include"ParticleManager.h"
//#include"CollisonManager.h"
#include "../Render/Camera.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include "../Collision/CollisonManager.h"

using namespace std;
class Player :public BaseObject
{
public:
	Player(Vector3 pos, Vector3 angle, ObjectManager* obj,shared_ptr<ModelRenderer>m,shared_ptr<ParticleManager>p,shared_ptr<TexRenderer>s);
	~Player();

	void Shot();
	// BaseObject を介して継承されました
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
private:
	ObjectManager* objM;
    shared_ptr<ModelRenderer>playerModel;
	shared_ptr<ParticleManager>playerParticle;
	shared_ptr<ParticleEmitterBox>playerParticleBox;
	shared_ptr<TexRenderer>playerSprite;
	Camera* camera;
	//カメラ用
	Vector3 CameraPos;
	Vector3 TargetPos;
	Vector3 CamVelocity;//カメラの移動量
	//制御用フラグ
	bool shotFlag =false;
	int shotcnt = 0;
	bool hitFlag = false;
	bool moveFlag;
	bool FrontMove;
	bool BackMove;
	Vector3 AIMPos;
	//ステータス
	int HP;
	bool GameOver = false;
	int bulletStock;//弾をいっぱいだすため
	float atkAngle;//砲塔用//描画
	float fireAngle;//砲身
	//サウンド

	// BaseObject を介して継承されました
	virtual void OnCollison(const CollisonInfo & info) override;

	// BaseObject を介して継承されました
	virtual void ImGuiDebug() override;
};