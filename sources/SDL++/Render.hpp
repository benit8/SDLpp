/*
** SDL++, 2020
** Render.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"
#include "Pixels.hpp"
#include "Rect.hpp"
#include "Surface.hpp"
#include "Texture.hpp"

#include <SDL2/SDL_render.h>

#include <utility>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Renderer
{
public:
	Renderer() = default;

	explicit Renderer(SDL_Renderer *renderer)
	: m_renderer{renderer}
	{}

	Renderer(const Renderer&) = delete;

	Renderer(Renderer &&other) noexcept
	{
		*this = std::move(other);
	}

	~Renderer()
	{
		SDL_DestroyRenderer(m_renderer);
	}

	////////////////////////////////////////////////////////////////////////////

	SDL_Renderer *ptr() const { return m_renderer; }

	void info(SDL_RendererInfo &info) const
	{
		if (SDL_GetRendererInfo(m_renderer, &info) != 0)
			throw Exception{"SDL_GetRendererInfo"};
	}

	SDL_RendererInfo info() const
	{
		SDL_RendererInfo i;
		info(i);
		return i;
	}

	Vec2i size() const
	{
		Vec2i s;
		if (SDL_GetRendererOutputSize(m_renderer, &s.x, &s.y) != 0)
			throw Exception{"SDL_GetRendererOutputSize"};
		return s;
	}

	Color drawColor() const
	{
		Color c;
		if (SDL_GetRenderDrawColor(m_renderer, &c.r, &c.g, &c.b, &c.a) != 0)
			throw Exception{"SDL_GetRenderDrawColor"};
		return c;
	}

	void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = SDL_ALPHA_OPAQUE) const
	{
		if (SDL_SetRenderDrawColor(m_renderer, r, g, b, a) != 0)
			throw Exception{"SDL_SetRenderDrawColor"};
	}

	void setDrawColor(const Color &c) const
	{
		setDrawColor(c.r, c.g, c.b, c.a);
	}

	Rect clipRect() const
	{
		Rect r;
		SDL_RenderGetClipRect(m_renderer, &r);
		return r;
	}

	void setClipRect(const Rect &r) const
	{
		if (SDL_RenderSetClipRect(m_renderer, &r) != 0)
			throw Exception{"SDL_RenderSetClipRect"};
	}

	bool isClipEnabled() const
	{
		return SDL_RenderIsClipEnabled(m_renderer);
	}

	void disableClip() const
	{
		if (SDL_RenderSetClipRect(m_renderer, nullptr) != 0)
			throw Exception{"SDL_RenderSetClipRect"};
	}

	bool intScale() const
	{
		return SDL_RenderGetIntegerScale(m_renderer);
	}

	void setIntScale(bool intscale) const
	{
		if (SDL_RenderSetIntegerScale(m_renderer, SDL_bool(intscale)) != 0)
			throw Exception{"SDL_RenderSetIntegerScale"};
	}

	void setBlendMode(SDL_BlendMode mode) const
	{
		if (SDL_SetRenderDrawBlendMode(m_renderer, mode) != 0)
			throw Exception{"SDL_RenderSetIntegerScale"};
	}


	Texture makeTexture(int w, int h, SDL_PixelFormatEnum format, SDL_TextureAccess access) const
	{
		return Texture{m_renderer, w, h, format, access};
	}

	Texture makeTexture(const Vec2i &size, SDL_PixelFormatEnum format, SDL_TextureAccess access) const
	{
		return Texture{m_renderer, size, format, access};
	}

	Texture makeTexture(const Surface &surface) const
	{
		return Texture{m_renderer, surface};
	}

	Texture makeTexture(const std::string &filename) const
	{
		return Texture{m_renderer, filename};
	}

	void copy(Texture &tex) const
	{
		SDL_RenderCopy(m_renderer, tex.ptr(), nullptr, nullptr);
	}

	void copy(Texture &tex, const Rect &source, const Rect &dest) const
	{
		SDL_RenderCopy(m_renderer, tex.ptr(), &source, &dest);
	}


	void present() const
	{
		SDL_RenderPresent(m_renderer);
	}

	void clear() const
	{
		if (SDL_RenderClear(m_renderer) != 0)
			throw Exception{"SDL_RenderClear"};
	}

	void clear(const Color &c) const
	{
		auto old = drawColor();
		setDrawColor(c);
		clear();
		setDrawColor(old);
	}


	void drawLine(const Vec2i &pos1, const Vec2i &pos2) const
	{
		if (SDL_RenderDrawLine(m_renderer, pos1.x, pos1.y, pos2.x, pos2.y) != 0)
			throw Exception{"SDL_RenderDrawLine"};
	}

	void drawLine(const Vec2i &pos1, const Vec2i &pos2, const Color &c) const
	{
		setDrawColor(c);
		drawLine(pos1, pos2);
	}

	void drawLines(const std::vector<Vec2i> &points) const
	{
		if (SDL_RenderDrawLines(m_renderer, &points[0], (int)points.size()) != 0)
			throw Exception{"SDL_RenderDrawLines"};
	}

	void drawLines(const std::vector<Vec2i> &points, const Color &c) const
	{
		setDrawColor(c);
		drawLines(points);
	}

	void drawPoint(const Vec2i &point) const
	{
		if (SDL_RenderDrawPoint(m_renderer, point.x, point.y) != 0)
			throw Exception{"SDL_RenderDrawPoint"};
	}

	void drawPoint(const Vec2i &point, const Color &c) const
	{
		setDrawColor(c);
		drawPoint(point);
	}

	void drawPoints(const std::vector<Vec2i> &points) const
	{
		if (SDL_RenderDrawPoints(m_renderer, &points[0], (int)points.size()) != 0)
			throw Exception{"SDL_RenderDrawPoints"};
	}

	void drawPoints(const std::vector<Vec2i> &points, const Color &c) const
	{
		setDrawColor(c);
		drawPoints(points);
	}

	void drawRay(const Vec2i &orig, const Vec2i &ray) const
	{
		drawLine(orig, orig + ray);
	}

	void drawRay(const Vec2i &orig, const Vec2i &ray, const Color &c) const
	{
		drawLine(orig, orig + ray, c);
	}

	void drawRect(const Rect &rect) const
	{
		if (SDL_RenderDrawRect(m_renderer, &rect) != 0)
			throw Exception{"SDL_RenderDrawRect"};
	}

	void drawRect(const Rect &rect, const Color &c) const
	{
		setDrawColor(c);
		drawRect(rect);
	}

	void drawRects(const std::vector<Rect> &rects) const
	{
		if (SDL_RenderDrawRects(m_renderer, &rects[0], (int)rects.size()) != 0)
			throw Exception{"SDL_RenderDrawRects"};
	}

	void drawRects(const std::vector<Rect> &rects, const Color& c) const
	{
		setDrawColor(c);
		drawRects(rects);
	}

	void fill() const
	{
		if (SDL_RenderFillRect(m_renderer, NULL) != 0)
			throw Exception{"SDL_RenderFillRect"};
	}

	void fill(const Color &c) const
	{
		setDrawColor(c);
		if (SDL_RenderFillRect(m_renderer, NULL) != 0)
			throw Exception{"SDL_RenderFillRect"};
	}

	void fillRect(const Rect &rect) const
	{
		if (SDL_RenderFillRect(m_renderer, &rect) != 0)
			throw Exception{"SDL_RenderFillRect"};
	}

	void fillRect(const Rect &rect, const Color &c) const
	{
		setDrawColor(c);
		fillRect(rect);
	}

	void fillRects(const std::vector<Rect> &rects) const
	{
		if (SDL_RenderFillRects(m_renderer, &rects[0], (int)rects.size()) != 0)
			throw Exception{"SDL_RenderDrawRects"};
	}

	void fillRects(const std::vector<Rect> &rects, const Color &c)
	{
		setDrawColor(c);
		fillRects(rects);
	}

	////////////////////////////////////////////////////////////////////////////

	Renderer &operator=(const Renderer&) = delete;

	Renderer &operator=(Renderer &&other) noexcept
	{
		if (m_renderer != other.m_renderer) {
			SDL_DestroyRenderer(m_renderer);
			m_renderer = other.m_renderer;
			other.m_renderer = nullptr;
		}
		return *this;
	}

private:
	SDL_Renderer *m_renderer = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

}