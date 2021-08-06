#pragma once

#include <string>

class ImGuiSimplified
{
public:
	//ImGuiSimplified() = 0;
	//~ImGuiSimplified() = 0;

	//�X���C�h�Œl���ŏ��l����ő�l�܂ŕύX�ł���
	static	bool imguiSliderFloat(std::string label, float* num, float min = 0.0f, float max = 1.0f);
	//�`�F�b�N�{�b�N�X
	static bool imguiCheckBox(std::string label, bool* isEneble);
	//�{�^��
	static bool imguiButton(std::string label);
	//���W�I�{�^��
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