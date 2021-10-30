#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"

//カメラに当たり判定など現在のstaticに付けずらい、常には必要ない機能を後付けする
class CameraEye:public BaseObject
{
public:
	CameraEye(Vector3 pos,Vector3 angle, ObjectManager * obj);
	~CameraEye();

private:


	// BaseObject を介して継承されました
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;
private:
	ObjectManager* objM;
};
