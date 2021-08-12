#pragma once

class SourceVoice;
class SoundPlayer;

class SoundPlayTimer
{
public:
	SoundPlayTimer(SourceVoice& sourceVoice, SoundPlayer& player);
	~SoundPlayTimer();

	//Ä¶ŠÔ‚Ìİ’è
	void setPlayTime(float time);
	//Ä¶‘¬“xİ’è
	void setFrequency(float getFrequency);
	//Ä¶ŠÔ‚ğŒJ‚è•Ô‚·
	float getPlayTime() const;
	//Ä¶ŠÔ‚ği‚ß‚é
	void startTimer();
	//Ä¶ŠÔ‚ğ~‚ß‚é
	void stopTimer();

private:
	SoundPlayTimer(const SoundPlayTimer&) = delete;
	SoundPlayTimer& operator=(const SoundPlayTimer&) = delete;

private:
	SourceVoice& mSourceVoice;
	SoundPlayer& mPlayer;
};