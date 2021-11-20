#include "TextEditer.h"
#include <vector>

TextEditor::TextEditor()
{
}

TextEditor::~TextEditor()
{
}

int TextEditor::strReplace(std::string & str, std::string from, std::string to)
{
	int n = 0;
	std::string::size_type pos = 0;
	while (pos = str.find(from, pos), pos != std::string::npos) {
		str.replace(pos, to.length(), to);
		pos += to.length();
		n++;
	}
	return n; // �u���񐔂�Ԃ�
}

void TextEditor::Read(std::string textName, std::vector<std::string>& vstr)
{
	std::ifstream ifs(textName);
	if (ifs.fail())
	{
		std::cout << "error :>> failed to open the text file." << std::endl;
		return;
	}
	std::string tmp;
	while (getline(ifs, tmp))
	{
		if ((tmp[0] == '/' && tmp[1] == '/') || tmp.empty())
			continue;
		else
		{
			strReplace(tmp, "\\", "/");//�p�X�̋�؂��u������
			vstr.push_back(tmp);
		}
	}
}

void TextEditor::Write(std::string textName, const std::vector<std::string>& vstr)
{
	std::ofstream file(textName);//�����o���t�@�C���p�X�̎w��
	
	for (const auto state : vstr)
	{
		file << state << std::endl;//�����o��
	}
}

void TextEditor::AddWrite(std::string textName, const std::vector<std::string>& vstr)
{
	std::ofstream file(textName, std::ios::app);//�����o���t�@�C���p�X�̎w��

	for (const auto state : vstr)
	{
		file << state << std::endl;//�����o��
	}
}
