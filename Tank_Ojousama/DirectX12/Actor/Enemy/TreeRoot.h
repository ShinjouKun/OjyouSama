#pragma once
#include "../BaseObject.h"
#include "../ObjectManager.h"
#include "../../Render/ModelRenderer.h"

/*�؂̍�����(�U���p)�N���X*/
class TreeRoot : public BaseObject
{
public:

	TreeRoot(
		const Vector3& pos,
		const Vector3& ang,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer> modelRender,
		int num
	);

	~TreeRoot();

	void SetPosition(const Vector3& value) { position = value; }

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* col) override;
	virtual void ImGuiDebug() override;

private:

	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;

	Vector3 mScale;

	string mModelNum;//�ԍ�
	string mRoot = "TreeRoot";	 //���O
	string mRootNum; //���O + �ԍ�
};