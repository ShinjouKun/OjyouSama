#pragma once

#include <string>

class FileUtil
{
	FileUtil() = default;
	~FileUtil() = default;

public:
	//�p�X����t�@�C���g���q���擾����
	static std::string getFileExtension(const std::string& filePath);

};