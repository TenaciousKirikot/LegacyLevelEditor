////////////////////////////////////////////////////////////
//
// MISSING COPYRIGHT
//
////////////////////////////////////////////////////////////

#ifndef EDITOR_COLOR_WINDOW_HPP
#define EDITOR_COLOR_WINDOW_HPP

#include "color_window_settings.hpp"
#include "hsv.hpp"
#include "../main/application_settings.hpp"
#include "../controls/numeric_up_down.hpp"
#include "../controls/slider.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <string>

class ColorWindow
{
public:
	ColorWindow();
	ColorWindow(sf::String title, SoundHandler* handler, std::map<sf::String, sf::String>* strings, ApplicationSettings* app_settings, ColorWindowSettings color_settings);

	bool								isOpen() const;
	void								closeWindow();
	void								hideWindow();
	void								showWindow(float r, float g, float b, function<void(float, float, float)> on_color_updated);
	void								updateLoop();
	void								updateVerticesLoop();
	void								renderLoop();
	void								setWindowSize(size_t index);
	void								setVsync(bool enable_vsync);
	void								setFramerateLimit(sf::Uint8 fps_limit);

private:
	bool								m_initialized;
	bool								m_active;
	bool								m_visible;
	float								m_r;
	float								m_g;
	float								m_b;
	function<void(float, float, float)>	m_on_color_updated;
	sf::RenderWindow*					m_window;
	sf::RectangleShape					m_icon;
	sf::VertexArray*					m_palette;
	sf::IntRect							m_palette_rect;
	int									m_palette_scale;
	int									m_palette_scale_global;
	Control**							m_controls;
	FloatNumeric*						m_r_numeric;
	FloatNumeric*						m_g_numeric;
	FloatNumeric*						m_b_numeric;
	FloatNumeric*						m_h_numeric;
	FloatNumeric*						m_s_numeric;
	FloatNumeric*						m_v_numeric;
	FloatSlider*						m_r_slider;
	FloatSlider*						m_g_slider;
	FloatSlider*						m_b_slider;
	FloatSlider*						m_h_slider;
	FloatSlider*						m_s_slider;
	FloatSlider*						m_v_slider;
	float								m_hue;
	bool								m_vertices_require_updating;
	std::tuple<sf::Uint16, sf::Uint16>* m_color_window_sizes;
	ApplicationSettings*				m_settings;
	SoundHandler*						m_sound_handler;

	float								updateColor();
	void								setVSyncOrFps();
};

#endif