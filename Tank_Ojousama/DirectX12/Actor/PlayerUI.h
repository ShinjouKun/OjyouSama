#pragma once
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include"ObjectManager.h"

class PlayerUI :public BaseObject
{
	//�d�l�ɂ���Ă͂��̃N���X�͕s�v�ɂȂ邩��
public:
	PlayerUI(ObjectManager* obj,shared_ptr<ModelRenderer>m, shared_ptr<TexRenderer>s);
	~PlayerUI();
	void Init();
private:
	
	//�����ɏ��󂯎��n��ǉ����Ă����iHP�⃌�[�_�[�Ȃǁj

private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>UIModel;
	shared_ptr<TexRenderer>UISprite;
	Vector3 AimPos;//�Ə��̈ʒu


				   // BaseObject ����Čp������܂���
	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(const CollisonInfo & info) override;

};