#pragma once

#include <string>

class ImGuiSimplified
{
public:
	//ImGuiSimplified() = 0;
	//~ImGuiSimplified() = 0;

	//スライドで値を最小値から最大値まで変更できる
	static	bool imguiSliderFloat(std::string label, float* num, float min = 0.0f, float max = 1.0f);
	//チェックボックス
	static bool imguiCheckBox(std::string label, bool* isEneble);
	//ボタン
	static bool imguiButton(std::string label);
	//ラジオボタン
	static bool imguiRadioButton(std::string label, int* v, int v_button);
	//SameLine
	static void imguiSameLine();
	//text
	static void imguiText(std::string text);
	//text
	static void imguiText(int text);
	//text
	static void imguiText(float text);
};