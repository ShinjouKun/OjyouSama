#pragma once
#include "BaseObject.h"             //オブジェクトのひな型
#include "ObjectManager.h"          //オブジェクト管理者
#include "BreadCrumb.h"             //パンくず
#include "../Render/ModelRenderer.h"//モデル貼り付け
#include "../Render/TexRenderer.h"	//ポリゴンの描画

class WayPoint : public BaseObject
{
public:
	/// <summary>
	/// コンストラクト
	/// </summary>
	/// <param name="position">初期位置</param>
	/// <param name="objManager">オブジェクト管理者</param>
	/// <param name="modelRender">モデル描画</param>
	/// <param name="num">識別番号</param>
	WayPoint(
		Vector3 position,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer> modelRender,
		int num
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WayPoint();
};