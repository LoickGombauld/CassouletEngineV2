#pragma once
#include  <SFML/Graphics.hpp>
#include  <CoraEngineLibrarie/Export.hpp>
#include <CoraEngineLibrarie/Shape.hpp>
#include  <CoraEngineLibrarie/Global.hpp>

class CASSOULET_DLL Window
{
public:
	Window(int width, int height,std::string);
	Window();
	Window(int width, int height);

	void Display();
	void Clear();
	void Clear(sf::Color color);
	void SetTitle(std::string title);
	bool isOpen() const;
	bool PollEvent(sf::Event& event);

	void UpdateMouse();
	void Close();
	void SetFrameRate(float);
	void SetVerticalSyncEnable(bool);
	void DrawCircleShape(Shape);
	void DrawRectShape(Shape);
	void DrawVertex(sf::VertexArray vertex);
	void SetMouseCursorGrabbed(bool isgrabbed);
	void SetMouseCursorVisible(bool isVisible);
	sf::RenderWindow* GetHandle();
	sf::Vector2u GetSize();
private:
	sf::RenderWindow m_window;
};