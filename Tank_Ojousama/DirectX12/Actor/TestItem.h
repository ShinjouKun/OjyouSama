#pragma once
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"

using namespace std;

class TestItem :public BaseObject
{
public:
	TestItem(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, int aliv, int n);
	~TestItem();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;

	ObjectManager* objM;
	shared_ptr<ModelRenderer>itemModel;
	//アイテム種類分け
	int number = 0;
	string name;
	string num;
	string numName;

	//生存時間指定用
	int alive;

	virtual void OnCollison(const CollisonInfo & info) override;

	virtual void ImGuiDebug() override;
};