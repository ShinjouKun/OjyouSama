#pragma once

#include "Sound3DListenerParam.h"
#include "../Sound3DCone.h"
#include "../../../Math/Math.h"

//3Dサウンドで扱うリスナー(聞き手)
class Sound3DListener
{
public:
	Sound3DListener();
	~Sound3DListener();

	//更新
	void update();
	//リスナーの位置を設定
	void setPosition(const Vector3& pos);
	//リスナーの前方ベクトルと上ベクトルを設定
	void setFrontAndTop(const Vector3& front, const Vector3& top);
	//コーンを設定する
	void setCone(Sound3DCone* cone);
	//リスナーを返す
	const Sound3DListenerParam& getListener() const;
	
private:
	//リスナー
	Sound3DListenerParam mListener;

	//前フレームでの位置
	Vector3 mPreviousPos;

	
	//リスナーコーン
	static  Sound3DCone LISTENER_DIRECTIONAL_CONE;
	//static inline Sound3DCone LISTENER_DIRECTIONAL_CONE = { Math::PI * 5.f / 6.f, Math::PI * 11.f / 6.f, 1.f, 0.75f, 0.f, 0.25f, 0.708f, 1.f };
};
