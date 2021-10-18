#pragma once
#include <memory>
#include "../Math/Vector3.h"

class TestBreadCrumb
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="position">�����ʒu</param>
	TestBreadCrumb(const Vector3& position, int breadNumber);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TestBreadCrumb();

	/*���Ԃ��o�����������悤�ɂ���*/
	void DeathInterval();

	/*�ʒu�擾*/
	Vector3 GetPosition() const;

	/*���ʔԍ��擾*/
	int GetBreadNumber() const;

	/*�������擾*/
	bool GetDeathFlag() const;

private:

	/*�ϐ��̏�����*/
	void Initialize();

private:

	Vector3 mPosition;

	int mBreadNumber;//���ʔԍ�
	int lifeTime;	 //��������
	int lifeCount;	 //�f�X�J�E���gGX

	bool mDeathFlag;
};