#pragma once
#include "BaseObject.h"
#include "ObjectManager.h"
#include "../Render/ModelRenderer.h"

class ElfRock :public BaseObject
{
public:
	ElfRock(const Vector3& pos, const Vector3& ang, ObjectManager* objManager, std::shared_ptr<ModelRenderer> modelRender, int num, int rockScale = 1);
	~ElfRock();

private:

	// BaseObject ����Čp������܂���
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
	int mRockScale;
	bool isActive;

	int number = 0;//���ʔԍ�
	string name;   //�L�[���[�h�o�^
	string num;    //�����p�i���o�[
	string numName;//�L�[���[�h+���ʔԍ�
};
