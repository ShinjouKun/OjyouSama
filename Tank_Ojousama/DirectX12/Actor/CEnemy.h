#pragma once
#include "BaseEnemy.h"
#include "../Render/ParticleManager.h"

class Timer;
class MemberEnemy;

class CEnemy : public BaseEnemy
{
public:
	CEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~CEnemy();

	//BaseEnemyから継承
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	virtual void Search() override;
	virtual void Warning() override;
	virtual void Attack() override;
	virtual void Destruct() override;

private:

	/*一度しか呼び出さない処理*/
	void OnlyOnceAction();

	/*隊員の移動管理*/
	void MemberMove(int number);

private:

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;
	shared_ptr<Timer> mSearchTimer;//メンバーがプレイヤーを検索する時間

	std::vector<MemberEnemy*> mMemberList;     //隊員リスト
	std::vector<Vector3> mMemberPositionList;//隊員のための位置リスト

	Vector3 mSearchPosition;//索敵結果位置

	enum SearchCommandStep
	{
		USUALLY,            //常時処理
		FIND_PLAYER,        //プレイヤーを見つけている
		SEARCH_ORDER,       //索敵開始を指示する
		RECEIVE_RESULT,     //索敵結果を受け取る
		MOVE_RESULTPOSITION,//報告位置に向かう
	};
	SearchCommandStep searchStep;

	bool mOnlyOnceTrigger;//一回しか呼びたくない処理用
	bool mAnyDeathFlag;   //リスト内の誰かが死んだか
	bool mSearchCommand;  //索敵指令
	bool mSearchResult;   //索敵の結果を受け取る

	int mEnemyNumber;//モデル描画用の番号

	const int MEMBER_COUNT = 2;//隊員の数

	const float SEARCH_INTERVAL = 1.5f;
	const float MEMBER_DISTANCE = 30.0f;//メンバーとの距離

	string mCircle;      //円のモデル
	string mCircleNumber;//円のモデル

	//歩行アニメーション関連
	float mLegRotate = 0.0f;//脚の現在の回転量
	const float LEG_SPEED = 1.5f; //脚の回転速度
	const float LEG_RANGE = 15.0f;//脚の最大回転量
	bool mRotDirection = false;   //回転方向

	string tankBarrel2;
	string numBarrel2;
};