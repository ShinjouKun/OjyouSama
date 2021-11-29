#pragma once
//#include"ObjectManager.h"
//#include"BaseObject.h"
//#include"ModelRenderer.h"
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"

using namespace std;
class Block :public BaseObject
{
public:
	Block(const Vector3& pos, const Vector3& ang, ObjectManager * objManager, shared_ptr<ModelRenderer> modelRender, int num);
	~Block();

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

	int number = 0;//���ʔԍ�
	string name;   //�L�[���[�h�o�^
	string num;    //�����p�i���o�[
	string numName;//�L�[���[�h+���ʔԍ�


	const float mRotValue = 10.0f;
	float rot = 0.0f;
};
