#pragma once

#include <xaudio2.h>

//�T�E���h�G�t�F�N�g�C���^�[�t�F�[�X
class ISoundEffect
{
public:
	virtual ~ISoundEffect() = default;
	/// <summary>
	/// �G�t�F�N�g�쐬�������z�֐�
	/// </summary>
	/// <param name="desc">XAPO�ݒ�p�f�X�N���v�^</param>
	/// <returns>�G�t�F�N�g�̍쐬�ɐ���������</returns>
	virtual bool create(XAUDIO2_EFFECT_DESCRIPTOR* desc) = 0;
};
