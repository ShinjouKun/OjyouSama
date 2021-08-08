#pragma once
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include"ObjectManager.h"

class PlayerUI :public BaseObject
{
	//仕様によってはこのクラスは不要になるかも
public:
	PlayerUI(ObjectManager* obj,shared_ptr<ModelRenderer>m, shared_ptr<TexRenderer>s);
	~PlayerUI();
	void Init();
private:
	
	//ここに情報受け取り系を追加していく（HPやレーダーなど）

private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>UIModel;
	shared_ptr<TexRenderer>UISprite;
	Vector3 AimPos;//照準の位置


				   // BaseObject を介して継承されました
	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(const CollisonInfo & info) override;

};