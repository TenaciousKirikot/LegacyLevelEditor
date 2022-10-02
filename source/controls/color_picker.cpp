////////////////////////////////////////////////////////////
//
// MISSING COPYRIGHT
// 
////////////////////////////////////////////////////////////

#include "color_picker.hpp"

using sf::Color;
using sf::IntRect;
using sf::RenderWindow;
using sf::Shader;
using sf::String;
using sf::Uint8;
using sf::Vector2u;
using std::map;

ColorPicker::ColorPicker() :

	Control::Control(),
	m_color_window(nullptr),
	m_r(0),
	m_g(0),
	m_b(0)
{

}

ColorPicker::ColorPicker(RenderWindow* window, Vector2u intended_resolution, map<String, String>* strings, ColorWindow* colow_window, const ControlTheme* theme, SoundHandler* handler, Vector2f position, Vector2f size,
	const IntRect& bg, const IntRect& bg_holding, const Color& color, float min_y_position, bool visible, bool enabled) :

	Control::Control(window, intended_resolution, strings, theme, handler, position, size, bg, bg_holding, min_y_position, visible, enabled),
	m_color_window(colow_window)
{
	setColor(color);
}

const Color& ColorPicker::getColor() const
{
	return Color(
		m_r,
		m_g,
		m_b
	);
}

void ColorPicker::setColor(const Color& color)
{
	m_r = color.r;
	m_g = color.g;
	m_b = color.b;
	m_bg->setColor(color);
	m_bg_active->setColor(color);
}

void ColorPicker::setColor(float r, float g, float b)
{
	m_r = r;
	m_g = g;
	m_b = b;

	Color color = Color(r, g, b);
	m_bg->setColor(color);
	m_bg_active->setColor(color);
}

void ColorPicker::setColorWindow(ColorWindow* color_window)
{
	m_color_window = color_window;
}

const Shader* ColorPicker::getShader() const
{
	return nullptr;
}

void ColorPicker::onMouseEntered(Event::MouseMoveEvent data)
{
	Control::onMouseEntered(data);
	m_drawable = m_holding ? m_bg_active : m_bg;
}

void ColorPicker::onMouseLeft(Event::MouseMoveEvent data)
{
	m_drawable = m_bg;
}

void ColorPicker::onClicked(Event::MouseButtonEvent data)
{
	Control::onClicked(data);
	m_drawable = m_bg_active;
}

void ColorPicker::onUnclicked(Event::MouseButtonEvent data)
{
	m_color_window->hideWindow();
}

void ColorPicker::onReleased(Event::MouseButtonEvent data)
{
	Control::onReleased(data);
	m_drawable = m_bg;

	m_shader = getShader();
	m_color_window->showWindow(
		m_r,
		m_g,
		m_b,
		[this](float r, float g, float b) 
		{
			setColor(r, g, b);
		}
	);
}