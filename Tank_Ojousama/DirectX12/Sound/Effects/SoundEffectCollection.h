#pragma once

#include "IEffectCreater.h"
#include "IEffectParameter.h"
#include "../Voice/IVoice.h"
#include <memory>

//class SoundFilter;

//�S�ẴG�t�F�N�g�ɃA�N�Z�X�ł���
class SoundEffectCollection
{
public:
	SoundEffectCollection(IVoice& voice, IEffectCreater& effectCreater, IEffectParameter& effectParameter);
	~SoundEffectCollection();

	//���o�[�u����
	int reverb();
	//�ȈՔŃ��o�[�u
	int simpleReverb();
	//�G�R�[��������
	int echo();
	//�C�R���C�U�[
	int equalizer();
	//�{�����[�����[�^�[
	int volumeMeter();
	//�Đ����Ԑݒ�/�擾
	void getPlayTimer();
	//�t�[���G�ϊ�
	int discreteFourierTransform();
	//�t�B���^�[�Ǘ��N���X��Ԃ�
	//SoundFilter& getFilter() const;
private:
	SoundEffectCollection(const SoundEffectCollection&) = delete;
	SoundEffectCollection& operator=(const SoundEffectCollection&) = delete;

public:
	static constexpr int PLAY_TIMER_ID = 0;

private:
	IEffectCreater& mEffectCreater;
	//std::unique_ptr<SoundFilter> mFilter;
};