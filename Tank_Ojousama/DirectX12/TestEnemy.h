#pragma once
#include <memory>
#include "BaseObject.h"//オブジェクトのひな型
#include "ObjectManager.h"//オブジェクト管理者
#include "ModelRenderer.h"//モデル貼り付け
#include "TexRenderer.h"//ポリゴンの描画

/// <summary>
/// 扇形の視界
/// </summary>
struct SenserFan
{
	Vector3 fanPoint;//中心点
	float fanArea;  //範囲
	float fanLength;//長さ
	float fanAngle;	//視野角
};

/// <summary>
/// プレイヤー追跡用センサー
/// </summary>
struct SenserPoint
{
	Vector3 hitPoint;  //当たった中心点
	float senserRadius;//半径
};

/// <summary>
/// 敵クラス
/// </summary>
class TestEnemy : public BaseObject
{

//関数系
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">初期位置</param>
	/// <param name="angle">初期角度</param>
	/// <param name="objManager">オブジェクト管理者</param>
	/// <param name="modelRender">モデル描画</param>
	/// <param name="texRender">ポリゴン描画</param>
	/// <param name="num">識別番号</param>
	TestEnemy(
		Vector3 position,
		Vector3 angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<TexRenderer>texRender,
		int num
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TestEnemy();

	/// <summary>
	/// 通常攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private:

	//BaseObjectから継承
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollision(const CollisonInfo & info) override;
	virtual void ImGuiDebug() override;

	/// <summary>
	/// 敵のベクトルを取得
	/// </summary>
	/// <param name="vec">敵の向き</param>
	/// <returns></returns>
	Vector3 GetEnemyVector(Vector3 vec);

//変数用
private:
	
	ObjectManager* mObj;
	shared_ptr<ModelRenderer> tankModel;
	shared_ptr<TexRenderer> tankSprite;
	Vector3 angleVector;
	SenserPoint senserPoint;

	//ステータス
	int HP;         //体力
	int attackCount;//攻撃のインターバル
	int hitCount;   //？
	int bulletStock;//弾の残量

#pragma region 複数出す用らしい

	int number = 0;   //識別番号
	int count = 0;    //?
	float attackAngle;//砲塔の描画用？
	float fireAngle;  //砲身の描画用？
	bool moveFlag;    //移動しているか
	bool frontMove;   //正面に移動しているか
	float backMove;   //後ろに移動しているか

	string tankAtk;   //キーワード登録
	string tankHou;   //?
	string tankBtm;   //した？
	string num;       //複数用ナンバー?
	string numNameAtk;//番号とキーワードの複合
	string numNameHou;
	string numNameBtm;

#pragma endregion

};