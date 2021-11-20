#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//using namespace std;

class TextEditor
{
public:
	TextEditor();
	~TextEditor();

	int strReplace(std::string &str, std::string from, std::string to);
	void Read(std::string textName, std::vector<std::string> &vstr);

	void Write(std::string textName, const std::vector<std::string> &vstr);
	void AddWrite(std::string textName,const std::vector<std::string> &vstr);

private:

};