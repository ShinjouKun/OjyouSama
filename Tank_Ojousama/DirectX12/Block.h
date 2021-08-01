#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include"ModelRenderer.h"

using namespace std;
class Block :public BaseObject
{
public:
	Block(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, int n);
	~Block();

private:


	// BaseObject ����Čp������܂���
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;


	ObjectManager* objM;
	shared_ptr<ModelRenderer>blockModel;
	//���������p
	int number = 0;
	string name;//�L�[���[�h�o�^
	string num;//�����p�i���o�[
	string numName;
	// BaseObject ����Čp������܂���
	virtual void OnCollison(const CollisonInfo & info) override;

	// BaseObject ����Čp������܂���
	virtual void ImGuiDebug() override;
	//���̕�����
};
