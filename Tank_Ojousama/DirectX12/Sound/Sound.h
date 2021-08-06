#pragma once
#include<memory>
#include<windows.h>
#include<xaudio2.h>
#include"wrl.h"
#pragma comment(lib,"xaudio2.lib")

using namespace Microsoft::WRL;


struct Chunk
{
	char id[4];//�`�����N����ID
	std::int32_t size;
};
struct RiffHeader
{
	Chunk chunk;//RIFF
	char type[4];
};
struct FormatChunk
{
	Chunk chunk;//fmt
	WAVEFORMAT fmt;

};

class XAudio2VoiceCallback :public IXAudio2VoiceCallback
{
public:
	//�T�E���h�����J�n��
	//STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	//�T�E���h�����I����
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
	//�o�b�t�@�X�g���[���Đ����I�������Ƃ�
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	//�o�b�t�@�̎g�p�J�n
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	//�o�b�t�@�̖����ɓ��B�����Ƃ�
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) { delete[]pBufferContext; };
	//�Đ������[�v�ʒu�ɒB������
	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContext) {};
	//�{�C�X�̎��s�G���[��
	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContext, HRESULT Error) {};
};

class Sound
{
public:
	Sound();
	~Sound();
	void Init();
	void LoadSE(const char*filename);//�Đ��i���j
	void LoadBGM(const char*filename);//
	void Play(const char*filename);
private:
	ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	IXAudio2SourceVoice* sourceVoice;
public:
	XAudio2VoiceCallback voiceCallback;//�R�[���o�b�N
};
