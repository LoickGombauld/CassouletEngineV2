#pragma once
#include <imgui.h>
#include <SFML/Graphics.hpp>

class IMGUISFPP
{
public:
	IMGUISFPP();
	~IMGUISFPP(); 
	ImGuiContext* GetContext();

	void Render() {

	}

	void NewFrame();

private:
	ImGuiContext* m_context;
};

IMGUISFPP::IMGUISFPP()
{
	IMGUI_CHECKVERSION();
	m_context = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
}

 IMGUISFPP::~IMGUISFPP()
{

}
