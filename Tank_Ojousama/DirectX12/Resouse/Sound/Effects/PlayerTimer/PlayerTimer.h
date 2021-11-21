#pragma once

#include "PlayTimerParam.h"
#include "../SoundEffectUtility.h"
#include <xapobase.h>

#pragma comment(lib, "xapobase.lib")

class __declspec(uuid("{16A1BD38-43A6-4822-8019-D5FA940215BB}"))
	PlayTimer : public CXAPOParametersBase
{
public:
	PlayTimer();
	~PlayTimer();

	//�ŏ��̈�񂾂��Ă΂��A�l��ێ����邽�߂̊֐�
	STDMETHOD(LockForProcess)(
		UINT32 InputLockedParameterCount,
		const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters,
		UINT32 OutputLockedParameterCount,
		const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters
		);

	//���b100�񏈗�����郁�C���֐�
	//���ۂɃG�t�F�N�g���|����Ƃ�
	STDMETHOD_(void, Process)(
		UINT32 InputProcessParameterCount,
		const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
		UINT32 OutputProcessParameterCount,
		XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
		BOOL IsEnabled
		);

	//�Đ����Ԃ�ݒ肷��(�P��: �b)
	STDMETHOD_(void, SetParameters)(
		const void* pParameters,
		UINT32 ParameterByteSize
		);

	//���݂̍Đ����Ԃ�Ԃ�(�P��: �b)
	STDMETHOD_(void, GetParameters)(
		void* pParameters,
		UINT32 ParameterByteSize
		);

private:
	PlayTimer(const PlayTimer&) = delete;
	PlayTimer& operator= (const PlayTimer&) = delete;

private:
	//�v���p�e�B
	static XAPO_REGISTRATION_PROPERTIES xapoRegProp_;
	

	//���̓t�H�[�}�b�g�̋L���ꏊ
	WAVEFORMATEX mInputFmt;
	//�o�̓t�H�[�}�b�g�̋L���ꏊ
	WAVEFORMATEX mOutputFmt;
	//�p�����[�^�̎�M�ꏊ
	PlayTimerParam mParams[EFFECT_PARAMETER_SIZE];

	//���݂̍Đ�����(�P��: �o�C�g)
	unsigned mCurrentTime;
};