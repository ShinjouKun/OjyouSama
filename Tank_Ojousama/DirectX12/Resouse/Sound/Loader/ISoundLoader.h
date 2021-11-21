#pragma once

#include <xaudio2.h>
#include <string>

//Interface for reading sound
class ISoundLoader {
public:
	enum class Seek {
		BEGIN = SEEK_SET, //From the beginning of the file
		CURRENT = SEEK_CUR, //From the current file position
		END = SEEK_END //From the end of file
	};

public:
	virtual ~ISoundLoader() = default;
	virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) = 0;
	virtual long read(BYTE** buffer, unsigned size) = 0;
	virtual void seek(long offset, Seek seek = Seek::CURRENT) = 0;
	virtual void seekBegin() = 0;
	virtual unsigned size() const = 0;
};