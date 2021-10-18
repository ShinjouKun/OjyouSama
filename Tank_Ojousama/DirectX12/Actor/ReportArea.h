#pragma once
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"
#include "../Collision/SpherCollider.h"

class ReportArea : public BaseObject
{
public:

	ReportArea(
		const Vector3 & pos,
		ObjectManager* objectManager,
		shared_ptr<ModelRenderer> modelRenderer,
		int num
	);

	~ReportArea();

	//表示状態の切り替え
	void SetActice(bool value);

	//ころす
	void SetDeath(bool value);

private:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* info) override;
	virtual void ImGuiDebug() override;

private:
	ObjectManager* objManager;
	shared_ptr<ModelRenderer> modelRender;

	Vector3 scale;
	Vector3 offsetScale;

	SphereCollider* spehereCollider;

	string name;   //モデル名
	string key;	   //識別番号
	string keyname;//名前と番号の合成

	float radius;//半径
	float reportRange;//報告範囲

	bool isActive;//表示状態

};