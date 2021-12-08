#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include"../Render/ModelRenderer.h"

class Treasure:public BaseObject
{
public:
	Treasure(const Vector3& pos, const Vector3& ang, ObjectManager* objManager, std::shared_ptr<ModelRenderer> modelRender);
	~Treasure();

private:


	// BaseObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;
private:
	bool get;
	Vector3 scale;
	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
};
