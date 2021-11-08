#pragma once
#include "../Math/Math.h"

class Transform3D
{
public:
	Transform3D();
	~Transform3D();

	//ワールド行列更新
	bool computeWorldTransform();
	//ワールド行列の取得
	Matrix4 getWorldTransform()const;
	//位置の設定
	void setPosition(const Vector3& pos);
	//位置の取得
	Vector3 getPosition()const;
	//translationの設定
	void translate(const Vector3& translation);
	//ワールドポジションの取得
	Vector3 getWorldPosition()const;

	//回転
	void setQuaternion(const Quaternion& rot);
	//回転量の取得
	Quaternion getQuaternion() const;

	//回転量
	void setRotation(const Vector3& axis);
	//回転量の取得
	Vector3 getRotation() const;

	//ピボット位置の設定
	void setPivot(const Vector3& pivot);
	//ピボット位置の取得
	Vector3 getPivot() const;

	//スケール値の設定
	void setScale(const Vector3& scale);
	//均等に拡縮する
	void setScale(float scale);
	//ローカルスケール値の取得
	Vector3 getScale() const;

	void setRadius(float r);
	float getRadius()const;


private:
	void shouldRecomputeTransform();

private:
	Matrix4 mWorldTransform;
	Vector3 mPosition;
	//Quaternion mRotation;
	Vector3 mRotation;
	Quaternion mRotQuaternion;
	Vector3 mPivot;
	Vector3 mScale;
	bool mIsRecomputeTransform;

	float mRadius;//半径(仮)
	bool mRotMatFlag;//Quaternionを使うかどうか
};