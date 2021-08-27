#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>

class SourceVoice;
class Sound3DListener;

class Sound
{
public:
	Sound(std::string filename, bool isCalculate3D = false);
	~Sound();
	//サウンド再生
	void play();
	//サウンドループ再生
	void playLoop();
	//一時停止
	void pause();
	//停止
	void stop();
	//音量の変更0〜100
	void setVol(float num);
	//リスナーのセット
	void setLisner(const Vector3& pos);
	//
	void setPos(const Vector3& pos);

private:
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;

private:
	std::string mFilename;
	std::shared_ptr<SourceVoice> mSourceVoice;
	std::shared_ptr<Sound3DListener> mListener;
};