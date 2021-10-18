#pragma once
#include <memory>
#include "../Math/Vector3.h"

class TestWayPoint
{
public:
	/// <summary>
	/// �R���X�g���N�g
	/// </summary>
	/// <param name="position">�����ʒu</param>
	TestWayPoint(const Vector3 & pos = Vector3().zero);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TestWayPoint();

	/*�S�Ẵt���O��������*/
	void FlagInit();

	/*�ʒu�擾*/
	Vector3 GetPosition();

	/*���łɌ����Ŏg�p������?*/
	bool GetUseFlag();

	/*��Q�������邩�H*/
	bool GetImpossibleFlag();

	/*����������?*/
	bool GetArrivalFlag();

	/*�i�s�s�\���H*/
	bool GetCloseFlag();

	/*�����������H�̒l��ύX*/
	void SetUseFlag(const bool value);

	/*�i�s�s�\���H�̒l��ύX*/
	void SetCloseFlag(const bool value);

	/*��Q�������邩�H�̒l��ύX*/
	void SetImpossibleFlag(const bool value);

private:

	std::shared_ptr<Vector3> mPosition;//�ʒu

	bool useFlag;       //�����ς݂��H
	bool arrivalFlag;	//�����������H(�g��Ȃ�)
	bool impossibleFlag;//��Q�������邩�H(�g��Ȃ�����)
	bool closeFlag;		//�i�s�s�\���H
};