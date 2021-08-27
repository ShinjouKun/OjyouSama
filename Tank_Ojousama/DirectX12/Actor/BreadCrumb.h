#pragma once
#include <memory>
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"

class BreadCrumb :public BaseObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="objManager">オブジェクト管理者</param>
	/// <param name="modelRender">モデル描画</param>
	/// <param name="num">識別番号</param>
	BreadCrumb(
		Vector3 pos,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer> modelRender,
		int num
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BreadCrumb();

	int GetNumber()
	{
		return keyNumber;
	}

	void SetPosition(Vector3 pos)
	{
		position = pos;
	}

private:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* info) override;
	virtual void ImGuiDebug() override;

private:
	ObjectManager* objectManager;
	shared_ptr<ModelRenderer> modelRender;
	ObjectType objectType;
	Vector3 scale;

	int lifeCount;//生存カウント
	int lifeTime; //生存時間

	float radius;//半径

	//複数出すよう
	int keyNumber = 0;
	string key;    //識別番号
	string name;   //モデル名
	string keyName;//モデル名+番号

};
