////////////////////////////////////////////////////////////
//
// MISSING COPYRIGHT
//
////////////////////////////////////////////////////////////

#ifndef EDITOR_COLOR_WINDOW_SETTINGS_HPP
#define EDITOR_COLOR_WINDOW_SETTINGS_HPP

#include "../themes/control_theme.hpp"
#include "../themes/input_control_theme.hpp"
#include "../themes/text_theme.hpp"
#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <functional>
#include <string>

struct ColorWindowSettings
{
	sf::Vector2u		intended_resolution;
	ControlTheme*		theme;
	TextTheme*			text_theme;
	InputControlTheme*	input_control_theme;
	sf::Vector2f		r_numeric_position;
	sf::Vector2f		g_numeric_position;
	sf::Vector2f		b_numeric_position;
	sf::Vector2f		h_numeric_position;
	sf::Vector2f		s_numeric_position;
	sf::Vector2f		v_numeric_position;
	sf::Vector2f		r_slider_position;
	sf::Vector2f		g_slider_position;
	sf::Vector2f		b_slider_position;
	sf::Vector2f		h_slider_position;
	sf::Vector2f		s_slider_position;
	sf::Vector2f		v_slider_position;
	sf::Vector2f		numeric_size;
	sf::Vector2f		numeric_button_size;
	sf::Vector2f		slider_size;
	sf::Vector2f		slider_thumb_size;
	sf::IntRect			numeric_bg_rect;
	sf::IntRect			numeric_bg_active_rect;
	sf::IntRect			numeric_top_button_bg_rect;
	sf::IntRect			numeric_top_button_bg_focused_rect;
	sf::IntRect			numeric_bottom_button_bg_rect;
	sf::IntRect			numeric_bottom_button_bg_focused_rect;
	sf::IntRect			slider_bg_rect;
	sf::IntRect			slider_bg_active_rect;
	sf::IntRect			slider_thumb_rect;
	float				scroll_multplier;
	bool				vertical;
	sf::Vector2f		vertex_position;
	int					vertex_scale;
	sf::IntRect			icon;
	int					scroll_time;
};

#endif