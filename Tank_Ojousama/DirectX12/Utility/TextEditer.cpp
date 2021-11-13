#include "TextEditer.h"

TextEditor::TextEditor()
{
}

TextEditor::~TextEditor()
{
}

int TextEditor::strReplace(string & str, string from, string to)
{
	int n = 0;
	string::size_type pos = 0;
	while (pos = str.find(from, pos), pos != string::npos) {
		str.replace(pos, to.length(), to);
		pos += to.length();
		n++;
	}
	return n; // �u���񐔂�Ԃ�
}

void TextEditor::Read(string textName, vector<string>& vstr)
{
	ifstream ifs(textName);
	if (ifs.fail())
	{
		cout << "error :>> failed to open the text file." << endl;
		return;
	}
	string tmp;
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

void TextEditor::write(string textName, vector<string>& vstr)
{
	ofstream file(textName);//�����o���t�@�C���p�X�̎w��
	
	for (const auto state : vstr)
	{
		file << state << endl;//�����o��
	}
}

void TextEditor::addWrite(string textName, vector<string>& vstr)
{
	ofstream file(textName, ios::app);//�����o���t�@�C���p�X�̎w��

	for (const auto state : vstr)
	{
		file << state << endl;//�����o��
	}
}