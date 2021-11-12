#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TextEditor
{
public:
	TextEditor();
	~TextEditor();

	int strReplace(string &str, string from, string to);
	void Read(string textName, vector<string> &vstr);

	void write(string textName, vector<string> &vstr);
	void addWrite(string textName, vector<string> &vstr);

private:

};