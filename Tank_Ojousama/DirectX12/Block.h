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


	// BaseObject を介して継承されました
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;


	ObjectManager* objM;
	shared_ptr<ModelRenderer>blockModel;
	//複数だす用
	int number = 0;
	string name;//キーワード登録
	string num;//複数用ナンバー
	string numName;
	// BaseObject を介して継承されました
	virtual void OnCollison(const CollisonInfo & info) override;

	// BaseObject を介して継承されました
	virtual void ImGuiDebug() override;
	//上二つの複合体
};
