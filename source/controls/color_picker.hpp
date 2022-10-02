////////////////////////////////////////////////////////////
//
// MISSING COPYRIGHT
// 
////////////////////////////////////////////////////////////

#ifndef EDITOR_COLOR_PICKER_HPP
#define EDITOR_COLOR_PICKER_HPP

#include "control.hpp"
#include "../utility/color_window.hpp"

class ColorPicker : public Control
{
public:
	ColorPicker();
	ColorPicker(sf::RenderWindow* window, sf::Vector2u intended_resolution, std::map<sf::String, sf::String>* strings, ColorWindow* colow_window, const ControlTheme* theme, SoundHandler* handler, Vector2f position, Vector2f size,
		const sf::IntRect& bg, const sf::IntRect& bg_holding, const sf::Color& color, float min_y_position, bool visible = true, bool enabled = true);

	const sf::Color&	getColor() const;
	void				setColor(const sf::Color& color);
	void				setColor(float r, float g, float b);
	void				setColorWindow(ColorWindow* color_window);

protected:
	ColorWindow*		m_color_window;
	float				m_r;
	float				m_g;
	float				m_b;

	const sf::Shader*	getShader() const override;
	void				onMouseEntered(Event::MouseMoveEvent data) override;
	void				onMouseLeft(Event::MouseMoveEvent data) override;
	void				onClicked(Event::MouseButtonEvent data) override;
	void				onUnclicked(Event::MouseButtonEvent data) override;
	void				onReleased(Event::MouseButtonEvent data) override;
};

#endif