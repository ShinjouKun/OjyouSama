#pragma once

#include <memory>
#include <string>

class SourceVoice;

class Sound
{
public:
	Sound(std::string filename);
	~Sound();
	//サウンド再生
	void play();
	//サウンドループ再生
	void playLoop();
	//3Dサウンド再生
	//void play3D();
	//3Dサウンドループ再生
	//void playLoop3D();
	//一時停止
	void pause();
	//停止
	void stop();
	//音量の変更0～100
	//void setVol(int num);

private:
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;

private:
	std::string mFilename;
	std::shared_ptr<SourceVoice> mSourceVoice;
};