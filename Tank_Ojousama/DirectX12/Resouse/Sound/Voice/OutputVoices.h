#pragma once

#include "IVoice.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

//�{�C�X�o�͊Ǘ�
class OutputVoices
{
public:
	OutputVoices(IVoice& voice);
	~OutputVoices();

	//�{�C�X�̏o�͐��ǉ�
	void addOutputVoice(const IVoice& voice, bool useFilter = false, bool isApply = true);
	//�Y���ɑΉ�����f�B�X�N���v�^��Ԃ�
	const XAUDIO2_SEND_DESCRIPTOR& getDesc(unsigned index) const;
	//�f�B�X�N���v�^�z��̃T�C�Y��Ԃ�
	size_t size() const;

private:
	OutputVoices(const OutputVoices&) = delete;
	OutputVoices& operator=(const OutputVoices&) = delete;

	//�ݒ��K�p����
	void apply();

private:
	IVoice& mVoice;
	std::vector<XAUDIO2_SEND_DESCRIPTOR> mDescs;

};