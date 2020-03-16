/*
** SDL++, 2020
** Video.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Render.hpp"
#include "Vec2.hpp"

#include <SDL2/SDL_video.h>
#include <string>

#ifdef CPP_SDL2_VK_WINDOW
	#include <SDL2/SDL_vulkan.h>
	#include <vulkan/vulkan.hpp>
#endif

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Window
{
public:
	Window() = default;

	Window(const std::string &title, const Vec2i &size, Uint32 flags = SDL_WINDOW_SHOWN)
	: m_window{SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, flags)}
	{
		if (!m_window)
			throw Exception{"SDL_CreateWindow"};
	}

	Window(Window&) = delete;

	Window(Window &&other) noexcept
	{
		*this = std::move(other);
	}

	virtual ~Window()
	{
		SDL_DestroyWindow(m_window);
	}

	////////////////////////////////////////////////////////////////////////////

	Renderer makeRenderer(Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) const
	{
		const auto render = SDL_CreateRenderer(m_window, -1, flags);
		if (!render)
			throw Exception{"SDL_CreateRenderer"};
		return Renderer{render};
	}

	int displayIndex() const
	{
		const auto r = SDL_GetWindowDisplayIndex(m_window);
		if (r == -1)
			throw Exception{"SDL_GetWindowDisplayIndex"};
		return r;
	}

	void setDisplayMode(const SDL_DisplayMode &mode) const
	{
		if (SDL_SetWindowDisplayMode(m_window, &mode) != 0)
			throw Exception{"SDL_SetWindowDisplayMode"};
	}

	SDL_DisplayMode displayMode() const
	{
		SDL_DisplayMode mode;
		if (SDL_GetWindowDisplayMode(m_window, &mode) != 0)
			throw Exception{"SDL_GetWindowDisplayMode"};
		return mode;
	}

	Uint32 flags() const { return SDL_GetWindowFlags(m_window); }

	void grab() { SDL_SetWindowGrab(m_window, SDL_TRUE); }
	void release() { SDL_SetWindowGrab(m_window, SDL_TRUE); }
	bool grabbed() const { return SDL_GetWindowGrab(m_window); }

	void setPosition(const Vec2i &v) { SDL_SetWindowPosition(m_window, v.x, v.y); }
	void move(const Vec2i &v) { setPosition(position() + v); }
	void resize(const Vec2i &newsize) const { SDL_SetWindowSize(m_window, newsize.x, newsize.y); }

	Vec2i position() const
	{
		Vec2i pos;
		SDL_GetWindowPosition(m_window, &pos.x, &pos.y);
		return pos;
	}

	Vec2i size() const
	{
		Vec2i s;
		SDL_GetWindowSize(m_window, &s.x, &s.y);
		return s;
	}

	void rename(const std::string &t) { SDL_SetWindowTitle(m_window, t.c_str()); }
	std::string title() const { return std::string{SDL_GetWindowTitle(m_window)}; }

	void setIcon(const Surface &icon) const
	{
		SDL_SetWindowIcon(m_window, icon.ptr());
	}

#ifdef SDLPP_USE_SDL_IMAGE
	void setIcon(const std::string &filename) const
	{
		auto icon = Surface{filename};
		SDL_SetWindowIcon(m_window, icon.ptr());
	}
#endif

	void hide() { SDL_HideWindow(m_window); }
	void maximize() { SDL_MaximizeWindow(m_window); }
	void minimize() { SDL_MinimizeWindow(m_window); }
	void raise() { SDL_RaiseWindow(m_window); }
	void restore() { SDL_RestoreWindow(m_window); }

	bool fullscreen() const
	{
		return flags() & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	void setFullscreen(bool fs)
	{
		if (SDL_SetWindowFullscreen(m_window, fs ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) != 0)
			throw Exception{"SDL_SetWindowFullscreen"};
	}

	void toggleFullscreen()
	{
		setFullscreen(!fullscreen());
	}


	SDL_Window *ptr() const { return m_window; }

	////////////////////////////////////////////////////////////////////////////

	Window &operator=(Window&) = delete;

	Window &operator=(Window &&other) noexcept
	{
		if (m_window != other.m_window) {
			SDL_DestroyWindow(m_window);
			m_window = other.m_window;
			other.m_window = nullptr;
		}
		return *this;
	}

private:
	SDL_Window *m_window = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

void showMessageBox(uint32_t flags, std::string &&title, std::string &&message);
void showMessageBox(uint32_t flags, std::string &&title, std::string &&message, const SDL::Window &parent);

////////////////////////////////////////////////////////////////////////////////

}