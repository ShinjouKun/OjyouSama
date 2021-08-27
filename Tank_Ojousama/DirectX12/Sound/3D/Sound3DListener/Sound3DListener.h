#pragma once

#include "Sound3DListenerParam.h"
#include "../Sound3DCone.h"
#include "../../../Math/Math.h"

//3D�T�E���h�ň������X�i�[(������)
class Sound3DListener
{
public:
	Sound3DListener();
	~Sound3DListener();

	//�X�V
	void update();
	//���X�i�[�̈ʒu��ݒ�
	void setPosition(const Vector3& pos);
	//���X�i�[�̑O���x�N�g���Ə�x�N�g����ݒ�
	void setFrontAndTop(const Vector3& front, const Vector3& top);
	//�R�[����ݒ肷��
	void setCone(Sound3DCone* cone);
	//���X�i�[��Ԃ�
	const Sound3DListenerParam& getListener() const;
	
private:
	//���X�i�[
	Sound3DListenerParam mListener;

	//�O�t���[���ł̈ʒu
	Vector3 mPreviousPos;

	
	//���X�i�[�R�[��
	static  Sound3DCone LISTENER_DIRECTIONAL_CONE;
	//static inline Sound3DCone LISTENER_DIRECTIONAL_CONE = { Math::PI * 5.f / 6.f, Math::PI * 11.f / 6.f, 1.f, 0.75f, 0.f, 0.25f, 0.708f, 1.f };
};
