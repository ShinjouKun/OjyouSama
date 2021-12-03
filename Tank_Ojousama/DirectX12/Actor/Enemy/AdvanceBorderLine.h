#pragma once
#include "../BaseObject.h"
#include "../ObjectManager.h"
#include "../../Render/ModelRenderer.h"

class AdvanceBorderLine : public BaseObject
{
public:

	AdvanceBorderLine(
		const Vector3& position,
		const Vector3& angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer> modelRender,
		int num,
		const Vector3& min = Vector3().zero,
		const Vector3& max = Vector3().one
	);

	~AdvanceBorderLine();

private:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* col) override;
	virtual void ImGuiDebug() override;

private:
	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;

	Vector3 mMin;
	Vector3 mMax;

	int number = 0;//識別番号
	string name;   //キーワード登録
	string num;    //複数用ナンバー
	string numName;//キーワード+識別番号
};