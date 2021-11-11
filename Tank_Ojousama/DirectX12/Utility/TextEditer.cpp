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
	return n; // 置換回数を返す
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
			strReplace(tmp, "\\", "/");//パスの区切りを置き換え
			vstr.push_back(tmp);
		}
	}
}

void TextEditor::write(string textName, vector<string>& vstr)
{
	ofstream file(textName);//書き出すファイルパスの指定
	
	for (const auto state : vstr)
	{
		file << state << endl;//書き出し
	}
}

void TextEditor::addWrite(string textName, vector<string>& vstr)
{
	ofstream file(textName, ios::app);//書き出すファイルパスの指定

	for (const auto state : vstr)
	{
		file << state << endl;//書き出し
	}
}
