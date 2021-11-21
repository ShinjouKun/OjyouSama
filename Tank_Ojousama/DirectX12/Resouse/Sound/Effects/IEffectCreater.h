#pragma once

#include "ISoundEffect.h"
#include <xaudio2.h>

//�G�t�F�N�g�쐬�C���^�[�t�F�[�X
class IEffectCreater
{
public:
	virtual ~IEffectCreater() = default;
	virtual int createEffect(ISoundEffect* target, bool isApply = true) = 0;
	virtual int createEffect(IUnknown* target, bool isApply = true) = 0;
};
