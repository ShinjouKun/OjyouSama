#pragma once

#include <list>
#include <memory>

class SourceVoice;
class SubmixVoice;
class MasteringVoice;
class Sound3DCalculator;
class Sound3DListener;

class SoundManager
{
public:
	SoundManager(const MasteringVoice& masteringVoice);
	~SoundManager();
	void update();
	void add(const std::shared_ptr<SourceVoice>& sound);
	void add(const std::shared_ptr<SubmixVoice>& submixVoice);
	void setListener(const std::shared_ptr<Sound3DListener>& listener);

private:
	std::list<std::shared_ptr<SourceVoice>> mSounds;
	std::list<std::shared_ptr<SubmixVoice>> mSubmixVoices;
	std::unique_ptr<Sound3DCalculator> mCalculator;
	std::shared_ptr<Sound3DListener> mListener;
};