#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include"../Render/ModelRenderer.h"

class Treasure:public BaseObject
{
public:
	Treasure(const Vector3& pos, const Vector3& ang, ObjectManager* objManager, std::shared_ptr<ModelRenderer> modelRender,int sceneNum);
	~Treasure();

private:


	// BaseObject ����Čp������܂���
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;
private:
	int sceneNum;
	bool get;
	Vector3 scale;
	Vector2 numPos;
	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
};
