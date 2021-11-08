#pragma once
#include "../Math/Math.h"

class Transform3D
{
public:
	Transform3D();
	~Transform3D();

	//���[���h�s��X�V
	bool computeWorldTransform();
	//���[���h�s��̎擾
	Matrix4 getWorldTransform()const;
	//�ʒu�̐ݒ�
	void setPosition(const Vector3& pos);
	//�ʒu�̎擾
	Vector3 getPosition()const;
	//translation�̐ݒ�
	void translate(const Vector3& translation);
	//���[���h�|�W�V�����̎擾
	Vector3 getWorldPosition()const;

	//��]
	void setQuaternion(const Quaternion& rot);
	//��]�ʂ̎擾
	Quaternion getQuaternion() const;

	//��]��
	void setRotation(const Vector3& axis);
	//��]�ʂ̎擾
	Vector3 getRotation() const;

	//�s�{�b�g�ʒu�̐ݒ�
	void setPivot(const Vector3& pivot);
	//�s�{�b�g�ʒu�̎擾
	Vector3 getPivot() const;

	//�X�P�[���l�̐ݒ�
	void setScale(const Vector3& scale);
	//�ϓ��Ɋg�k����
	void setScale(float scale);
	//���[�J���X�P�[���l�̎擾
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

	float mRadius;//���a(��)
	bool mRotMatFlag;//Quaternion���g�����ǂ���
};