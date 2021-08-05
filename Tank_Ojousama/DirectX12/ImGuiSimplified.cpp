#include "ImGuiSimplified.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"
#include "ImGuiSimplified.h"

bool ImGuiSimplified::imguiSliderFloat(std::string label, float * num, float min, float max)
{
	return ImGui::SliderFloat(label.c_str(), num, min, max);
}

bool ImGuiSimplified::imguiCheckBox(std::string label, bool * isEneble)
{
	return ImGui::Checkbox(label.c_str(), isEneble);
}

bool ImGuiSimplified::imguiButton(std::string label)
{
	return ImGui::Button(label.c_str());
}

bool ImGuiSimplified::imguiRadioButton(std::string label, int * v, int v_button)
{
	return ImGui::RadioButton(label.c_str(), v, v_button);
}

void ImGuiSimplified::imguiSameLine()
{
	ImGui::SameLine();
}

void ImGuiSimplified::imguiText(std::string text)
{
	ImGui::Text(text.c_str());
}

void ImGuiSimplified::imguiText(int text)
{
	ImGui::Text("%d", text);
}

void ImGuiSimplified::imguiText(float text)
{
	ImGui::Text("%.3f", text);
}
