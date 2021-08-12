#pragma once

#include <list>
#include <memory>

class SourceVoice;
class MasteringVoice;

class SoundManager
{
public:
	SoundManager(const MasteringVoice& masteringVoice);
	~SoundManager();
	void update();
	void add(const std::shared_ptr<SourceVoice>& sound);

private:
	std::list<std::shared_ptr<SourceVoice>> mSounds;
};