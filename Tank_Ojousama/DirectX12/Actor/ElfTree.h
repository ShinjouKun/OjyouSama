#pragma once
#include "BaseObject.h"
#include "ObjectManager.h"
#include "../Render/ModelRenderer.h"

class ElfTree :public BaseObject
{
public:
	ElfTree(const Vector3& pos, const Vector3& ang, ObjectManager* objManager, std::shared_ptr<ModelRenderer> modelRender, int num);
	~ElfTree();

private:

	// BaseObject を介して継承されました
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* col) override;
	virtual void ImGuiDebug() override;

private:

	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;

	Vector3 scale;
	float radius;
	bool isActive;

	int number = 0;//識別番号
	string name;   //キーワード登録
	string num;    //複数用ナンバー
	string numName;//キーワード+識別番号
};
