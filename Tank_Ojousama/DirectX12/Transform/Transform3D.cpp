#include "Transform3D.h"

#include <iostream>
#include <cassert>

Transform3D::Transform3D() :
	mWorldTransform(Matrix4::Identity),
	mPosition(Vector3::zero),
	mRotation(Vector3::zero),
	mPivot(Vector3::zero),
	mScale(Vector3::one),
	mIsRecomputeTransform(true),
	mRadius(1.0f),
	mRotQuaternion(Quaternion::identity),
	mRotMatFlag(false)
{
}

Transform3D::~Transform3D()
{
}

bool Transform3D::computeWorldTransform()
{
	if (mIsRecomputeTransform)
	{
		mWorldTransform = Matrix4::createTranslation(-mPivot);//中心+ピボットを原点に
		mWorldTransform *= Matrix4::createScale(getScale());
		if (mRotMatFlag)
		{
			//
		}
		else
		{
			mWorldTransform *= Matrix4::RotateZ(mRotation.z);
			mWorldTransform *= Matrix4::RotateY(mRotation.y);
			mWorldTransform *= Matrix4::RotateX(mRotation.x);
		}
		mWorldTransform *= Matrix4::createTranslation(getPosition());
		mIsRecomputeTransform = false;
	}
	return true;
}

Matrix4 Transform3D::getWorldTransform() const
{
	return mWorldTransform;
}

void Transform3D::setPosition(const Vector3 & pos)
{
	mPosition = pos;
	shouldRecomputeTransform();
}

Vector3 Transform3D::getPosition() const
{
	return mPosition;
}

void Transform3D::translate(const Vector3 & translation)
{
	mPosition += translation;
	shouldRecomputeTransform();
}

Vector3 Transform3D::getWorldPosition() const
{
	return mPosition;
}

void Transform3D::setQuaternion(const Quaternion & rot)
{
	mRotQuaternion = rot;
	mRotMatFlag = true;
	shouldRecomputeTransform();
}

Quaternion Transform3D::getQuaternion() const
{
	return mRotQuaternion;
}

void Transform3D::setRotation(const Vector3 & axis)
{
	mRotation = axis;
	shouldRecomputeTransform();
}

Vector3 Transform3D::getRotation() const
{
	return mRotation;
}

void Transform3D::setPivot(const Vector3 & pivot)
{
	mPivot = pivot;
	shouldRecomputeTransform();
}

Vector3 Transform3D::getPivot() const
{
	return mPivot;
}

void Transform3D::setScale(const Vector3 & scale)
{
	mScale = scale;
	shouldRecomputeTransform();
}

void Transform3D::setScale(float scale)
{
	mScale.x = scale;
	mScale.y = scale;
	mScale.z = scale;
	shouldRecomputeTransform();
}

Vector3 Transform3D::getScale() const
{
	return mScale;
}

void Transform3D::setRadius(float r)
{
	mRadius = r;
	shouldRecomputeTransform();
}

float Transform3D::getRadius() const
{
	return mRadius;
}

void Transform3D::shouldRecomputeTransform()
{
	mIsRecomputeTransform = true;
}
