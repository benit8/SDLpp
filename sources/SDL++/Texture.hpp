/*
** SDL++, 2020
** Texture.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"
#include "Pixels.hpp"
#include "Rect.hpp"
#include "Surface.hpp"
#include "Vec2.hpp"

#include <SDL2/SDL_render.h>

#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Texture
{
public:
	class Lock
	{
	private:
		Lock(SDL_Texture *texture, const SDL_Rect *rect)
		: m_texture{texture}
		{
			if (SDL_LockTexture(m_texture, rect, &m_pixels, &m_pitch) != 0)
				throw Exception{"SDL_LockTexture"};

			Uint32 f = 0;
			SDL_QueryTexture(m_texture, &f, nullptr, &m_size.x, &m_size.y);
			m_format = SDL_AllocFormat(f);
			if (!m_format)
				throw Exception{"SDL_AllocFormat"};
		}

	public:
		~Lock() {
			SDL_UnlockTexture(m_texture);
			SDL_FreeFormat(m_format);
		}

		Pixel at(size_t x, size_t y) const {
			return Pixel{static_cast<Uint8*>(m_pixels) + (y * m_pitch) + (x * m_format->BytesPerPixel), *m_format};
		}

		Pixel at(const Vec2i &pos) const {
			return at(pos.x, pos.y);
		}

		Pixel operator[](const Vec2i &pos) const {
			return at(pos.x, pos.y);
		}

		Uint8 *pixels() { return static_cast<Uint8*>(m_pixels); }
		const Uint8 *pixels() const { return static_cast<Uint8*>(m_pixels); }
		int pitch() const { return m_pitch; }
		int width() const { return m_size.x; }
		int height() const { return m_size.y; }
		const Vec2i &size() const { return m_size; }

	private:
		friend class Texture;

		SDL_Texture *m_texture = nullptr;
		void *m_pixels = nullptr;
		int m_pitch = 0;
		Vec2i m_size;
		SDL_PixelFormat* m_format = nullptr;
	};

	////////////////////////////////////////////////////////////////////////////

	Texture() = default;

	explicit Texture(SDL_Texture *t)
	: m_texture{t}
	{}

	Texture(SDL_Renderer *render, int w, int h, SDL_PixelFormatEnum format = SDL_PIXELFORMAT_ARGB32, SDL_TextureAccess access = SDL_TEXTUREACCESS_STREAMING)
	: Texture{SDL_CreateTexture(render, format, access, w, h)}
	{
		if (!m_texture)
			throw Exception{"SDL_CreateTexture"};
	}

	Texture(SDL_Renderer *render, const Vec2i &size, SDL_PixelFormatEnum format = SDL_PIXELFORMAT_ARGB32, SDL_TextureAccess access = SDL_TEXTUREACCESS_STREAMING)
	: Texture{render, size.x, size.y, format, access}
	{}

	Texture(SDL_Renderer *render, const Surface &surface)
	: Texture{SDL_CreateTextureFromSurface(render, surface.ptr())}
	{
		if (!m_texture)
			throw Exception{"SDL_CreateTextureFromSurface"};
	}

	Texture(SDL_Renderer *render, const std::string &filename)
	: Texture{render, Surface{filename}}
	{}

	Texture(const Texture &) = delete;

	Texture(Texture &&other) noexcept
	{
		*this = std::move(other);
	}

	~Texture()
	{
		SDL_DestroyTexture(m_texture);
	}

	////////////////////////////////////////////////////////////////////////////

	void update(const void *pixels, int pitch)
	{
		if (SDL_UpdateTexture(m_texture, NULL, pixels, pitch) != 0)
			throw Exception{"SDL_UpdateTexture"};
	}

	void update(const void *pixels, const SDL_Rect &rect, int pitch)
	{
		if (SDL_UpdateTexture(m_texture, &rect, pixels, pitch) != 0)
			throw Exception{"SDL_UpdateTexture"};
	}

	void setBlendMode(const SDL_BlendMode &bm) const
	{
		if (SDL_SetTextureBlendMode(m_texture, bm) != 0)
			throw Exception{"SDL_SetTextureBlendMode"};
	}

	SDL_BlendMode blendMode() const
	{
		SDL_BlendMode bm;
		if (SDL_GetTextureBlendMode(m_texture, &bm) != 0)
			throw Exception{"SDL_GetTextureBlendMode"};
		return bm;
	}

	void setColorMod(const Color &color) const
	{
		setColorMod(color.r, color.g, color.b);
	}

	void setColorMod(Uint8 r, Uint8 g, Uint8 b) const
	{
		if (SDL_SetTextureColorMod(m_texture, r, g, b) != 0)
			throw Exception{"SDL_SetTextureColorMod"};
	}

	Color colorMod() const
	{
		Color c;
		if (SDL_GetTextureColorMod(m_texture, &c.r, &c.g, &c.b) != 0)
			throw Exception{"SDL_SetTextureColorMod"};
		return c;
	}

	void setAlphaMod(Uint8 alpha) const
	{
		if (SDL_SetTextureAlphaMod(m_texture, alpha) != 0)
			throw Exception{"SDL_SetTextureAlphaMod"};
	}

	Uint8 alphaMod() const
	{
		Uint8 alpha;
		if (SDL_GetTextureAlphaMod(m_texture, &alpha) != 0)
			throw Exception{"SDL_GetTextureAlphaMod"};
		return alpha;
	}

	void setColorAlphaMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
	{
		setColorMod(r, g, b);
		setAlphaMod(a);
	}

	void setColorAlphaMod(const Color &c) const
	{
		setColorMod(c);
		setAlphaMod(c.a);
	}

	Color colorAlphaMod() const
	{
		auto c = colorMod();
		c.a	= alphaMod();
		return c;
	}

	Uint32 format() const
	{
		Uint32 f = 0;
		if (SDL_QueryTexture(m_texture, &f, nullptr, nullptr, nullptr) != 0)
			throw Exception{"SDL_QueryTexture"};
		return f;
	}

	int access() const
	{
		int a = 0;
		if (SDL_QueryTexture(m_texture, nullptr, &a, nullptr, nullptr) != 0)
			throw Exception{"SDL_QueryTexture"};
		return a;
	}

	Vec2i size() const
	{
		Vec2i s;
		if (SDL_QueryTexture(m_texture, nullptr, nullptr, &s.x, &s.y) != 0)
			throw Exception{"SDL_QueryTexture"};
		return s;
	}

	Lock lock() { return Lock{m_texture, nullptr}; }
	Lock lock(const Rect &rect) { return Lock{m_texture, &rect}; }

	SDL_Texture *ptr() const { return m_texture; }

	////////////////////////////////////////////////////////////////////////////

	Texture &operator=(const Texture &) = delete;

	Texture &operator =(Texture &&other) noexcept
	{
		if (m_texture != other.m_texture) {
			SDL_DestroyTexture(m_texture);
			m_texture= other.m_texture;
			other.m_texture = nullptr;
		}
		return *this;
	}

private:
	SDL_Texture *m_texture = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

}