#pragma once

#include <string>

class FileUtil
{
	FileUtil() = default;
	~FileUtil() = default;

public:
	//パスからファイル拡張子を取得する
	static std::string getFileExtension(const std::string& filePath);

};