//#pragma once
//#include "BaseObject.h"             //オブジェクトのひな型
//#include "ObjectManager.h"          //オブジェクト管理者
//#include "../Render/ModelRenderer.h"
//
//class WayPoint : public BaseObject
//{
//public:
//	/// <summary>
//	/// コンストラクト
//	/// </summary>
//	/// <param name="position">初期位置</param>
//	/// <param name="objManager">オブジェクト管理者</param>
//	/// <param name="modelRender">モデル描画</param>
//	/// <param name="num">識別番号</param>
//	WayPoint(
//		const Vector3 & pos,
//		ObjectManager * objManager,
//		shared_ptr<ModelRenderer> modelRender,
//		int num
//	);
//
//	//デフォルトコンストラクタ
//	WayPoint();
//
//	/// <summary>
//	/// デストラクタ
//	/// </summary>
//	~WayPoint();
//
//	void FlagInit();
//
//	bool GetUseFlag() { return useFlag; }
//	bool GetImpossibleFlag() { return impossibleFlag; }
//	bool GetArrivalFlag() { return arrivalFlag; }
//	bool GetCloseFlag() { return closeFlag; }
//
//	void SetUseFlag(bool value) { useFlag = value; }
//	void SetCloseFlag(bool value) { closeFlag = value; }
//	void SetImpossibleFlag(bool value) { impossibleFlag = value; }
//	void SetDeathFlag(bool value) { death = value; }
//
//private:
//	virtual void Init() override;
//	virtual void Update() override;
//	virtual void Rend() override;
//	virtual void OnCollison(BaseCollider* col) override;
//	virtual void ImGuiDebug() override;
//
//private:
//
//	ObjectManager* objectManager;
//	shared_ptr<ModelRenderer> modelRenderer;
//
//	int keyNumber = 0;
//
//	bool useFlag;
//	bool arrivalFlag;
//	bool impossibleFlag;
//	bool closeFlag;
//
//	string key;	   //
//	string name;   //
//	string keyName;//
//};