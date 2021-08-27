#pragma once
#include "IEffectCreater.h"
#include "IEffectParameter.h"
#include "ISoundEffect.h"
#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

class SoundEffectCollection;

class SoundEffect :public IEffectCreater, public IEffectParameter
{
public:
	SoundEffect(IVoice& voice);
	~SoundEffect();

	//�G�t�F�N�g�Ƀp�����[�^�[��ݒ�
	virtual void setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize) override;

	//�G�t�F�N�g�̃p�����[�^�[���擾
	virtual void getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const override;

	//�G�t�F�N�g��L��������
	void enable(int effectID);

	//�G�t�F�N�g�𖳌�������
	void disable(int effectID);

	//�G�t�F�N�g�̗L������Ԃ�Ԃ�
	bool getEnabled(int effectID);

	//�G�t�F�N�g�W�ɃA�N�Z�X����
	SoundEffectCollection& getEffectCollection() const;

private:
	SoundEffect(const SoundEffect&) = delete;
	SoundEffect& operator=(const SoundEffect&) = delete;

	//�G�t�F�N�g�𐶐�����
	virtual int createEffect(ISoundEffect* target, bool isApply = true) override;
	virtual int createEffect(IUnknown* target, bool isApply = true) override;
	//�쐬�����S�G�t�F�N�g��K�p����
	void apply();
	//ID���L����
	bool isValidID(int effectID) const;
	//�p�����[�^���L����
	bool isValidParameters(const void* parameters) const;
	//�G�t�F�N�g�ɃA�N�Z�X�ł��邩
	bool canAccessEffect(int effectID, const void* parameters) const;

private:
	IVoice& mVoice;
	std::unique_ptr<SoundEffectCollection> mEffectCollection;
	std::vector<XAUDIO2_EFFECT_DESCRIPTOR> mDescs;
	bool mIsApplied;
};