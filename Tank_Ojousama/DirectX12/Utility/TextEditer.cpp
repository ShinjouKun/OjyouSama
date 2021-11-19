#include "TextEditer.h"
#include <vector>

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
	ifstream ifs;

	ifs.open(textName, ios::in);
	if (ifs.fail())
	{
		cout << "error :>> failed to open the text file." << endl;
		return;
	}
	string tmp;
	while (getline(ifs, tmp))
	{
		if ((tmp[0] == '/' && tmp[1] == '/') || tmp.empty())
		{
			ifs.close();
			continue;
		}
		else
		{
			strReplace(tmp, "\\", "/");//�p�X�̋�؂��u������
			vstr.push_back(tmp);
		}
	}
}

void TextEditor::Write(string textName, const vector<string>& vstr)
{
	ofstream file;//�����o���t�@�C���p�X�̎w��
	
	file.open(textName,ios::out);

	for (const auto state : vstr)
	{
		file << state << endl;//�����o��
	}
	file.close();
}

void TextEditor::AddWrite(string textName, const vector<string>& vstr)
{
	ofstream file(textName, ios::app);//�����o���t�@�C���p�X�̎w��

	for (const auto state : vstr)
	{
		file << state << endl;//�����o��
	}
}
