#pragma once
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"

class AttackArea : public BaseObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="ang">角度</param>
	/// <param name="objM">オブジェクト管理者</param>
	/// <param name="modelR">モデル描画</param>
	/// <param name="num">識別番号</param>
	AttackArea(
		Vector3 pos,
		Vector3 ang,
		ObjectManager* objM,
		shared_ptr<ModelRenderer>modelR,
		int num
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AttackArea();

	void SetPosition(Vector3 pos);

	void SetActive(bool value,Vector3 pos = Vector3(0,0,0),Vector3 ang = Vector3(0, 0, 0));

	bool GetActive();

	void SetDeath(bool value);

private:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(const CollisonInfo & info) override;
	virtual void ImGuiDebug() override;

private:
	ObjectManager* objManager;
	shared_ptr<ModelRenderer> modelRender;
	Vector3 scale;

	int number = 0;

	float radius;//半径

	bool isActive;//表示状態

	string name;   //モデル名
	string key;	   //識別番号
	string keyname;//名前と番号の合成
};