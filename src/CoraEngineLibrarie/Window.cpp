#include <CoraEngineLibrarie/Window.hpp>


Window::Window(int width, int height, std::string windowName) : m_window(sf::RenderWindow(sf::VideoMode(width, height), windowName))
{
}

Window::Window() : m_window(sf::RenderWindow(sf::VideoMode(800, 600), ""))
{
}

Window::Window(int width, int height) : m_window(sf::RenderWindow(sf::VideoMode(width, height), ""))
{
}
void Window::Display() { m_window.display(); }
void Window::Clear() { m_window.clear(); }
void Window::Clear(sf::Color color) { m_window.clear(color); }

void Window::SetTitle(std::string title) { m_window.setTitle(title); }

bool Window::isOpen() const
{
	return m_window.isOpen();
}

bool Window::PollEvent(sf::Event& event)
{
	return m_window.pollEvent(event);
}

void Window::Close()
{
	m_window.close();
}

void Window::SetFrameRate(float fps)
{
	m_window.setFramerateLimit(fps);
}

void Window::DrawCircleShape(Shape shape)
{
	m_window.draw(shape.GethandleCircle());
}

void Window::DrawRectShape(Shape shape)
{
	m_window.draw(shape.GethandleRect());
}

void Window::DrawVertex(sf::VertexArray vertex)
{
	m_window.draw(vertex);
}

void Window::SetMouseCursorGrabbed(bool isgrabbed)
{
	m_window.setMouseCursorGrabbed(isgrabbed);
}

void Window::SetMouseCursorVisible(bool isVisible)
{
	m_window.setMouseCursorVisible(isVisible);
}

sf::RenderWindow* Window::GetHandle()
{
	return &m_window;
}

sf::Vector2u Window::GetSize()
{
	return m_window.getSize();
}


void Window::SetVerticalSyncEnable(bool isactive)
{
	m_window.setVerticalSyncEnabled(isactive);
}
