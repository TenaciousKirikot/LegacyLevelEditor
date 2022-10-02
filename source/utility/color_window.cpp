////////////////////////////////////////////////////////////
//
// MISSING COPYRIGHT
// 
////////////////////////////////////////////////////////////

#include "color_window.hpp"

using sf::Color;
using sf::RenderWindow;
using sf::String;
using sf::Vector2u;
using sf::Vertex;
using sf::VertexArray;
using sf::VideoMode;
using sf::Uint8;
using sf::Uint16;
using std::function;
using std::get;
using std::min;
using std::map;
using std::max;
using std::tuple;
using std::make_tuple;
using namespace sf::Style;

ColorWindow::ColorWindow() :
	m_window(nullptr),
	m_initialized(false),
	m_active(false),
	m_visible(false),
	m_controls(nullptr),
	m_r_numeric(nullptr),
	m_g_numeric(nullptr),
	m_b_numeric(nullptr),
	m_h_numeric(nullptr),
	m_s_numeric(nullptr),
	m_v_numeric(nullptr),
	m_r_slider(nullptr),
	m_g_slider(nullptr),
	m_b_slider(nullptr),
	m_h_slider(nullptr),
	m_s_slider(nullptr),
	m_v_slider(nullptr),
	m_palette_scale(1),
	m_palette_scale_global(256),
	m_palette(),
	m_r(0),
	m_g(0),
	m_b(0),
	m_hue(0),
	m_vertices_require_updating(false)
{

}

ColorWindow::ColorWindow(String title, SoundHandler* handler, map<String, String>* strings, ApplicationSettings* app_settings, ColorWindowSettings color_settings) :
	m_settings(app_settings),
	m_initialized(true),
	m_active(false),
	m_visible(true),
	m_palette_scale(color_settings.vertex_scale),
	m_palette_scale_global(color_settings.vertex_scale * 256),
	m_r(0),
	m_g(0),
	m_b(0),
	m_hue(0),
	m_vertices_require_updating(false),
	m_color_window_sizes(
		new tuple<Uint16, Uint16>[2]
		{
			make_tuple(500, 500),
			make_tuple(1000, 1000)
		}
	),
	m_sound_handler(handler)
{
	auto window_size = m_color_window_sizes[m_settings->window_size > 1 ? 0 : m_settings->window_size];
	Uint16 width = m_settings->full_screen ? 1920 : get<0>(window_size);
	Uint16 height = m_settings->full_screen ? 1080 : get<1>(window_size);

	m_window = new RenderWindow(VideoMode(width, height, 64), title, Close);
	m_window->setVerticalSyncEnabled(app_settings->color_window_enable_vsync);
	m_window->setFramerateLimit(app_settings->color_window_fps_limit);
	m_window->setActive(false);

	m_icon.setPosition(color_settings.icon.left, color_settings.icon.top);
	m_icon.setSize(Vector2f(color_settings.icon.width, color_settings.icon.height));

	auto constructColor = [&, this](FloatNumeric** numeric, FloatSlider** slider, float max, Vector2f numeric_position, Vector2f slider_position, function<void(float)> on_updated)
	{
		*numeric = new FloatNumeric(
			m_window,
			color_settings.intended_resolution,
			strings,
			color_settings.theme,
			m_sound_handler,
			color_settings.text_theme,
			color_settings.input_control_theme,
			numeric_position,
			color_settings.numeric_size,
			color_settings.numeric_bg_rect,
			color_settings.numeric_bg_active_rect,
			color_settings.numeric_button_size,
			color_settings.numeric_top_button_bg_rect,
			color_settings.numeric_top_button_bg_focused_rect,
			color_settings.numeric_bottom_button_bg_rect,
			color_settings.numeric_bottom_button_bg_focused_rect,
			0,
			max,
			1,
			0,
			[&, this, slider, on_updated](float value)
			{
				(*slider)->forceSetValue(value);
				on_updated(value);
				m_on_color_updated(m_r, m_g, m_b);
				m_icon.setFillColor(Color(m_r, m_g, m_b));
			},
			color_settings.scroll_time,
			-1
		);

		*slider = new FloatSlider(
			m_window,
			color_settings.intended_resolution,
			strings,
			color_settings.theme,
			m_sound_handler,
			slider_position,
			color_settings.slider_size,
			color_settings.slider_bg_rect,
			color_settings.slider_bg_active_rect,
			color_settings.slider_thumb_size,
			color_settings.slider_thumb_rect,
			0,
			max,
			0,
			color_settings.scroll_multplier,
			[&, this, numeric, on_updated](float value)
			{
				(*numeric)->forceSetValue(value);
				on_updated(value);
				m_on_color_updated(m_r, m_g, m_b);
				m_icon.setFillColor(Color(m_r, m_g, m_b));
			},
			color_settings.vertical,
			-1
		);
	};

	auto constructRgb = [&, this, constructColor](FloatNumeric** numeric, FloatSlider** slider, Vector2f numeric_position, Vector2f slider_position, float* pointer)
	{
		constructColor(
			numeric,
			slider,
			255.,
			numeric_position,
			slider_position,
			[&, this, pointer](float value)
			{
				*pointer = value;
				
				Hsv hsv = Hsv(m_r, m_g, m_b);
				m_hue = hsv.h / 360.f;
				m_vertices_require_updating = true;

				m_h_numeric->forceSetValue(hsv.h);
				m_s_numeric->forceSetValue(hsv.s);
				m_v_numeric->forceSetValue(hsv.v);
				m_h_slider->forceSetValue(hsv.h);
				m_s_slider->forceSetValue(hsv.s);
				m_v_slider->forceSetValue(hsv.v);
			}
		);
	};

	auto constructHsv = [&, this]<typename T>(T max, FloatNumeric** numeric, FloatSlider** slider,  Vector2f numeric_position, Vector2f slider_position, function<T* (Hsv&)> action)
	{
		constructColor(
			numeric,
			slider,
			static_cast<T>(max),
			numeric_position,
			slider_position,
			function<void(T)>
			(
				[&, this, action](T value)
				{
					Hsv hsv = Hsv(m_r, m_g, m_b);
					*action(hsv) = value;

					RGBA rgba = hsv.toRGB();
					m_r = get<0>(rgba);
					m_g = get<1>(rgba);
					m_b = get<2>(rgba);

					m_hue = hsv.h / 360.f;
					m_vertices_require_updating = true;

					m_r_numeric->forceSetValue(m_r);
					m_g_numeric->forceSetValue(m_g);
					m_b_numeric->forceSetValue(m_b);
					m_r_slider->forceSetValue(m_r);
					m_g_slider->forceSetValue(m_g);
					m_b_slider->forceSetValue(m_b);
				}
			)
		);
	};

	constructRgb(&m_r_numeric, &m_r_slider, color_settings.r_numeric_position, color_settings.r_slider_position, &m_r);
	constructRgb(&m_g_numeric, &m_g_slider, color_settings.g_numeric_position, color_settings.g_slider_position, &m_g);
	constructRgb(&m_b_numeric, &m_b_slider, color_settings.b_numeric_position, color_settings.b_slider_position, &m_b);
	constructHsv(
		static_cast<Uint16>(360),
		&m_h_numeric,
		&m_h_slider,
		color_settings.h_numeric_position,
		color_settings.h_slider_position,
		function<Uint16* (Hsv&)>([](Hsv& hsv) { return &hsv.h; })
	);
	constructHsv(
		static_cast<Uint8>(100),
		&m_s_numeric,
		&m_s_slider,
		color_settings.s_numeric_position,
		color_settings.s_slider_position,
		function<Uint8* (Hsv&)>([](Hsv& hsv) { return &hsv.s; })
	);
	constructHsv(
		static_cast<Uint8>(100),
		&m_v_numeric,
		&m_v_slider,
		color_settings.v_numeric_position,
		color_settings.v_slider_position,
		function<Uint8* (Hsv&)>([](Hsv& hsv) { return &hsv.v; })
	);

	m_controls = new Control*[12] {
		m_r_numeric, m_g_numeric, m_b_numeric, m_h_numeric, m_s_numeric, m_v_numeric,
		m_r_slider, m_g_slider, m_b_slider, m_h_slider, m_s_slider, m_v_slider
	};

	m_palette = new VertexArray(sf::Points, m_palette_scale_global * m_palette_scale_global);

	for (size_t y = 0; y < m_palette_scale_global; ++y)
	{
		for (size_t x = 0; x < m_palette_scale_global; ++x)
		{
			Vertex& vert = m_palette->operator[](y * m_palette_scale_global + x);
			vert.position.x = color_settings.vertex_position.x + m_palette_scale_global - 1 - x;
			vert.position.y = color_settings.vertex_position.y + m_palette_scale_global - 1 - y;
			vert.color = Color::Black;
		}
	}

	m_palette_rect = IntRect(
		color_settings.vertex_position.x,
		color_settings.vertex_position.y,
		m_palette_scale_global,
		m_palette_scale_global
	);
}

bool ColorWindow::isOpen() const
{
	return m_window->isOpen();
}

void ColorWindow::closeWindow()
{
	m_window->close();
}

void ColorWindow::hideWindow()
{
	m_active = false;
}

void ColorWindow::showWindow(float r, float g, float b, function<void(float, float, float)> on_color_updated)
{
	m_active = true;
	m_r = r;
	m_g = g;
	m_b = b;
	m_on_color_updated = on_color_updated;

	m_hue = updateColor();
	m_vertices_require_updating = true;
}

void ColorWindow::updateVerticesLoop()
{
	if (m_vertices_require_updating)
	{
		m_vertices_require_updating = false;
		size_t x = 0;

		for (size_t j = 0; j < 256; ++j)
		{
			for (size_t b = 0; b < m_palette_scale; ++b)
			{
				for (size_t i = 0; i < 256; ++i)
				{
					for (size_t a = 0; a < m_palette_scale; ++a)
					{
						const double s = i / 255.;
						const double v = j / 255.;
						const int cs = m_hue * 6;

						const double f = m_hue * 6 - cs;
						const double p = v * (1 - s);
						const double q = v * (1 - s * f);
						const double t = v * (1 - s * (1 - f));

						Color color;
						switch (cs)
						{
							case 0:
							case 6:
							{
								color = sf::Color(v * 255, t * 255, p * 255);
								break;
							}
							case 1:
							{
								color = sf::Color(q * 255, v * 255, p * 255);
								break;
							}
							case 2:
							{
								color = sf::Color(p * 255, v * 255, t * 255);
								break;
							}
							case 3:
							{
								color = sf::Color(p * 255, q * 255, v * 255);
								break;
							}
							case 4:
							{
								color = sf::Color(t * 255, p * 255, v * 255);
								break;
							}
							case 5:
							{
								color = sf::Color(v * 255, p * 255, q * 255);
								break;
							}
						}

						m_palette->operator[](x).color = color;
						++x;
					}
				}
			}
		}
	}
}

void ColorWindow::updateLoop()
{
	for (Control** pointer = m_controls; pointer < m_controls + 12; ++pointer)
	{
		(*pointer)->onUpdated();
	}

	Event window_event = Event();
	while (m_window->pollEvent(window_event))
	{
		if (window_event.type == sf::Event::Closed)
		{
			hideWindow();
		}
		else if (window_event.type == sf::Event::MouseButtonReleased)
		{
			Event::MouseButtonEvent button = window_event.mouseButton;
			bool contains = m_palette_rect.contains(button.x, button.y);

			if (contains)
			{
				int x = button.x - m_palette_rect.left;
				int y = button.y - m_palette_rect.top;

				Color color = m_palette->operator[](m_palette_scale_global* m_palette_scale_global - y * m_palette_scale_global - x).color;
				m_r = color.r;
				m_g = color.g;
				m_b = color.b;

				updateColor();
				m_on_color_updated(m_r, m_g, m_b);
			}
		}

		bool valid = true;
		for (Control** pointer = m_controls; pointer < m_controls + 12 && valid; ++pointer)
		{
			valid = (*pointer)->onWindowEvent(window_event);
		}
	}
}

void ColorWindow::renderLoop()
{
	if (m_visible != m_active)
	{
		m_visible = m_active;
		m_window->setVisible(m_visible);
	}

	if (m_active)
	{
		m_window->clear();
		for (Control** pointer = m_controls; pointer < m_controls + 12; ++pointer)
		{
			m_window->draw(**pointer);
		}

		m_window->draw(m_icon);
		m_window->draw(*m_palette);
		m_window->display();
	}
}

void ColorWindow::setWindowSize(size_t index)
{
	auto size_tuple = m_color_window_sizes[index];
	m_settings->color_window_size = index;

	float width = get<0>(size_tuple);
	float height = get<1>(size_tuple);
	m_window->setSize(Vector2u(width, height));

	Event size_event = Event();
	size_event.type = Event::Resized;
	size_event.size = Event::SizeEvent();
	size_event.size.width = width;
	size_event.size.height = height;

	for (Control** pointer = m_controls; pointer < m_controls + 12; ++pointer)
	{
		(*pointer)->onWindowEvent(size_event);
	}
}

void ColorWindow::setVsync(bool enable_vsync)
{
	m_settings->color_window_enable_vsync = enable_vsync;
	setVSyncOrFps();
}

void ColorWindow::setFramerateLimit(sf::Uint8 fps_limit)
{
	m_settings->color_window_fps_limit = fps_limit;
	setVSyncOrFps();
}

float ColorWindow::updateColor()
{
	m_r_numeric->forceSetValue(m_r);
	m_g_numeric->forceSetValue(m_g);
	m_b_numeric->forceSetValue(m_b);
	m_r_slider->forceSetValue(m_r);
	m_g_slider->forceSetValue(m_g);
	m_b_slider->forceSetValue(m_b);

	Hsv hsv = Hsv(m_r, m_g, m_b);
	m_h_numeric->forceSetValue(hsv.h);
	m_s_numeric->forceSetValue(hsv.s);
	m_v_numeric->forceSetValue(hsv.v);
	m_h_slider->forceSetValue(hsv.h);
	m_s_slider->forceSetValue(hsv.s);
	m_v_slider->forceSetValue(hsv.v);

	m_icon.setFillColor(Color(m_r, m_g, m_b));
	return hsv.h / 360.;
}

void ColorWindow::setVSyncOrFps()
{
	if (m_settings->color_window_enable_vsync)
	{
		m_window->setVerticalSyncEnabled(true);
	}
	else
	{
		m_window->setFramerateLimit(m_settings->color_window_fps_limit);
	}
}