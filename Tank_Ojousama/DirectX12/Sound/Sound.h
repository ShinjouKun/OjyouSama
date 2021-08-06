#pragma once
#include<memory>
#include<windows.h>
#include<xaudio2.h>
#include"wrl.h"
#pragma comment(lib,"xaudio2.lib")

using namespace Microsoft::WRL;


struct Chunk
{
	char id[4];//チャンク毎のID
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
	//サウンド処理開始時
	//STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	//サウンド処理終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
	//バッファストリーム再生が終了したとき
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	//バッファの使用開始
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	//バッファの末尾に到達したとき
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) { delete[]pBufferContext; };
	//再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContext) {};
	//ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContext, HRESULT Error) {};
};

class Sound
{
public:
	Sound();
	~Sound();
	void Init();
	void LoadSE(const char*filename);//再生（一回）
	void LoadBGM(const char*filename);//
	void Play(const char*filename);
private:
	ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	IXAudio2SourceVoice* sourceVoice;
public:
	XAudio2VoiceCallback voiceCallback;//コールバック
};
