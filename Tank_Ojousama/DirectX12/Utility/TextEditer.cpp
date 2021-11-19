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
	return n; // 置換回数を返す
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
			strReplace(tmp, "\\", "/");//パスの区切りを置き換え
			vstr.push_back(tmp);
		}
	}
}

void TextEditor::Write(string textName, const vector<string>& vstr)
{
	ofstream file;//書き出すファイルパスの指定
	
	file.open(textName,ios::out);

	for (const auto state : vstr)
	{
		file << state << endl;//書き出し
	}
	file.close();
}

void TextEditor::AddWrite(string textName, const vector<string>& vstr)
{
	ofstream file(textName, ios::app);//書き出すファイルパスの指定

	for (const auto state : vstr)
	{
		file << state << endl;//書き出し
	}
}
