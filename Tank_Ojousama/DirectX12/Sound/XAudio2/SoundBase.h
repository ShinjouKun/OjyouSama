#pragma once

class XAudio2;
class MasteringVoice;

//Sound top management class
class SoundBase
{
public:
	SoundBase();
	~SoundBase();

	XAudio2& getXAudio2()const;

	MasteringVoice& getMasteringVoice()const;

	bool isNull() const;

private:
	SoundBase(const SoundBase&) = delete;
	SoundBase& operator=(const SoundBase&) = delete;

	//COMƒVƒXƒeƒ€‰Šú‰»
	bool comInitialize();

private:
	//Raw pointer to control the deletion order
	XAudio2* mXAudio2;
	MasteringVoice* mMasteringVoice;
	bool mSucceedInitialize;

	static bool mInstantiated;
};
