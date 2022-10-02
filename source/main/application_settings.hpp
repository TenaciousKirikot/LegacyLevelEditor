////////////////////////////////////////////////////////////
//
// MISSING COPYRIGHT
//
////////////////////////////////////////////////////////////

#ifndef EDITOR_APPLICATION_SETTINGS_HPP
#define EDITOR_APPLICATION_SETTINGS_HPP

#include <SFML/Config.hpp>
#include <SFML/System/String.hpp>

struct ApplicationSettings
{
	size_t				window_size;
	bool				full_screen;
	bool				enable_vsync;
	sf::Uint8			fps_limit;
	size_t				color_window_size;
	bool				color_window_enable_vsync;
	sf::Uint8			color_window_fps_limit;
	sf::String			language;
	sf::String			theme;
	sf::String			color_window;
	sf::String			layout;
	float				music_volume;
	float				sound_volume;
	bool				use_pretty_printing;
	bool				use_tabs;
};

#endif