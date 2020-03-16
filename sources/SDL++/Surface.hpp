/*
** SDL++, 2020
** Surface.hpp
*/

#pragma once

#define SDLPP_USE_SDL_IMAGE

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"
#include "Pixels.hpp"
#include "Rect.hpp"
#include "Vec2.hpp"

#include <SDL2/SDL_surface.h>
#ifdef SDLPP_USE_SDL_IMAGE
	#include <SDL2/SDL_image.h>
#endif

#include <string>
#include <utility>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Surface
{
public:
	class Lock
	{
	private:
		constexpr explicit Lock(SDL_Surface &m_surface)
		: m_surface{&m_surface}
		{}

	public:
		~Lock() {
			SDL_UnlockSurface(m_surface);
		}

		Pixel at(const Vec2i &pos) const {
			return at(pos.x, pos.y);
		}

		Pixel at(size_t x, size_t y) const {
			return Pixel{static_cast<Uint8*>(m_surface->pixels) + (y * m_surface->pitch) + (x * m_surface->format->BytesPerPixel), *m_surface->format};
		}

		Pixel operator[](const Vec2i &pos) const {
			return at(pos.x, pos.y);
		}

		void *rawArray() const {
			return m_surface->pixels;
		}

	private:
		friend class Surface;

		SDL_Surface *m_surface;
	};

	////////////////////////////////////////////////////////////////////////////

	explicit Surface(SDL_Surface *surface)
	: m_surface{surface}
	{}

	explicit Surface(int w, int h, int depth = 32, Uint32 format = SDL_PIXELFORMAT_ARGB32)
	: m_surface{SDL_CreateRGBSurfaceWithFormat(0, w, h, depth, format)}
	{
		if (!m_surface)
			throw Exception{"SDL_CreateRGBSurfaceWithFormat"};
	}

	explicit Surface(const Vec2i &size, int depth = 32, Uint32 format = SDL_PIXELFORMAT_ARGB32)
	: Surface(0, size.x, size.y, depth, format)
	{}

	explicit Surface(void *pixels, int w, int h, int depth = 32, Uint32 format = SDL_PIXELFORMAT_ARGB32)
	: m_surface{SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, depth, depth / 8 * w, format)}
	{
		if (!m_surface)
			throw Exception{"SDL_CreateRGBSurfaceWithFormatFrom"};
	}

	explicit Surface(void *pixels, const Vec2i &size, int depth = 32, Uint32 format = SDL_PIXELFORMAT_ARGB32)
	: Surface(pixels, size.x, size.y, depth, format)
	{}

	Surface(Surface &&other) noexcept
	{
		*this = std::move(other);
	}

#ifdef SDLPP_USE_SDL_IMAGE
	explicit Surface(const std::string &filename)
	: m_surface{IMG_Load(filename.c_str())}
	{
		if (!m_surface)
			throw Exception{"IMG_Load"};
	}
#else
	explicit Surface(const std::string&)
	: m_surface{nullptr}
	{
		SDL_SetError("Tried to call SDL::Surface(const std::string &filename) ctor. This function should call IMG_Load() from SDL_Image.\nThis program was built without SDL_Image.\nPlease Install SDL_Image and #define SDLPP_USE_SDL_IMAGE before including SDL.hpp to use this functionality");
		throw Exception("IMG_Load");
	}
#endif

	~Surface()
	{
		SDL_FreeSurface(m_surface);
	}

	////////////////////////////////////////////////////////////////////////////

	Surface withFormat(const SDL_PixelFormat &format) const
	{
		auto s = SDL_ConvertSurface(m_surface, &format, 0);
		if (!s)
			throw Exception{"SDL_ConvertSurface"};
		return Surface{s};
	}

	Surface withFormat(Uint32 format) const
	{
		auto s = SDL_ConvertSurfaceFormat(m_surface, format, 0);
		if (!s)
			throw Exception{"SDL_ConvertSurfaceFormat"};
		return Surface{s};
	}

	Surface &convertTo(const SDL_PixelFormat &format)
	{
		return *this = withFormat(format);
	}

	Surface &convertTo(Uint32 format)
	{
		return *this = withFormat(format);
	}

#if SDL_VERSION_ATLEAST(2, 0, 9)
	bool hasColorKey() const
	{
		return SDL_HasColorKey(m_surface) == SDL_TRUE;
	}
#endif

	void blitOn(const Rect &src, Surface &surf, const Rect &dst) const
	{
		auto dstmut = const_cast<Rect&>(dst);
		if (SDL_BlitSurface(m_surface, &src, surf.m_surface, &dstmut) != 0)
			throw Exception{"SDL_BlitSurface"};
	}

	void blitOn(Surface &surf, const Rect &dst) const
	{
		auto dstmut = const_cast<Rect&>(dst);
		if (SDL_BlitSurface(m_surface, nullptr, surf.m_surface, &dstmut) != 0)
			throw Exception{"SDL_BlitSurface"};
	}

	Vec2i size() const { return Vec2i{width(), height()}; }
	int width() const { return m_surface->w; }
	int height() const { return m_surface->h; }

	const SDL_PixelFormat &pixelFormat() const { return *m_surface->format; }
	Uint32 format() const { return m_surface->format->format; }
	Uint32 flags() const { return m_surface->flags; }

	Rect clipRect() const
	{
		Rect r;
		SDL_GetClipRect(m_surface, &r);
		return r;
	}

	void disableColorKey() const
	{
		if (SDL_SetColorKey(m_surface, SDL_FALSE, 0) != 0)
			throw Exception{"SDL_SetColorKey"};
	}

	void setColorKey(Uint32 key) const
	{
		if (SDL_SetColorKey(m_surface, SDL_TRUE, key) != 0)
			throw Exception{"SDL_SetColorKey"};
	}

	void setColorKey(const Color &color) const
	{
		if (SDL_SetColorKey(m_surface, SDL_TRUE, color.asUint(pixelFormat())) != 0)
			throw Exception{"SDL_SetColorKey"};
	}

	Color colorKey() const
	{
		Uint32 k;
		if (SDL_GetColorKey(m_surface, &k) != 0)
			throw Exception{"SDL_GetColorKey"};
		return Color{k, pixelFormat()};
	}

	void setBlendMode(const SDL_BlendMode &bm) const
	{
		if (SDL_SetSurfaceBlendMode(m_surface, bm) != 0)
			throw Exception{"SDL_SetSurfaceBlendMode"};
	}

	SDL_BlendMode blendMode() const
	{
		SDL_BlendMode bm;
		if (SDL_GetSurfaceBlendMode(m_surface, &bm) != 0)
			throw Exception{"SDL_GetSurfaceBlendMode"};
		return bm;
	}

	void setColorMod(const Color &color) const
	{
		setColorMod(color.r, color.g, color.b);
	}

	void setColorMod(Uint8 r, Uint8 g, Uint8 b) const
	{
		if (SDL_SetSurfaceColorMod(m_surface, r, g, b))
			throw Exception{"SDL_SetSurfaceColorMod"};
	}

	Color colorMod() const
	{
		Color c;
		if (SDL_GetSurfaceColorMod(m_surface, &c.r, &c.g, &c.b) != 0)
			throw Exception{"SDL_SetSurfaceColorMod"};
		return c;
	}

	void setAlphaMod(Uint8 alpha) const
	{
		if (SDL_SetSurfaceAlphaMod(m_surface, alpha) != 0)
			throw Exception{"SDL_SetSurfaceAlphaMod"};
	}

	Uint8 alphaMod() const
	{
		Uint8 alpha;
		if (SDL_GetSurfaceAlphaMod(m_surface, &alpha) != 0)
			throw Exception{"SDL_GetSurfaceAlphaMod"};
		return alpha;
	}

	void setColorAlphaMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
	{
		setColorMod(r, g, b);
		setAlphaMod(a);
	}

	void setColorAlphaMod(const Color &c) const
	{
		setColorMod(c.r, c.g, c.b);
		setAlphaMod(c.a);
	}

	Color colorAlphaMod() const
	{
		auto c = colorMod();
		c.a	= alphaMod();
		return c;
	}

	Lock lock() const
	{
		if (SDL_LockSurface(m_surface) != 0)
			throw Exception{"SDL_LockSurface"};
		return Lock{*m_surface};
	}

	SDL_Surface *ptr() const { return m_surface; }

	////////////////////////////////////////////////////////////////////////////

	Surface &operator =(Surface &&other) noexcept
	{
		if (m_surface != other.m_surface) {
			SDL_FreeSurface(m_surface);
			m_surface = other.m_surface;
			other.m_surface = nullptr;
		}
		return *this;
	}

private:
	SDL_Surface *m_surface = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

}