/*
** SDL++, 2020
** Timer.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"

#include <SDL2/SDL_timer.h>

#include <cassert>
#include <chrono>
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Timer
{
	explicit Timer(SDL_TimerID timer)
	: m_timer{timer}
	{}

public:
	using Callback = SDL_TimerCallback;

	Timer() = delete;

	bool remove()
	{
		if (m_timer > 0)
			return SDL_RemoveTimer(m_timer);
		else
			return false;
	}

	SDL_TimerID timerId() const
	{
		return m_timer;
	}

	////////////////////////////////////////////////////////////////////////////

	static Timer create(uint32_t interval, Callback function, void *user_context)
	{
		SDL_TimerID id = SDL_AddTimer(interval, function, user_context);
		if (!id)
			throw Exception("SDL_AddTimer");
		return Timer(id);
	}

	static Timer create(std::chrono::milliseconds interval, Callback function, void *user_context)
	{
		return create(static_cast<uint32_t>(interval.count()), function, user_context);
	}

	static void delay(std::chrono::milliseconds millisec)
	{
		assert(millisec.count() >= 0);
		delay(static_cast<uint32_t>(millisec.count()));
	}

	static void delay(uint32_t millisec)
	{
		SDL_Delay(millisec);
	}

	static uint32_t ticksU32()
	{
		return static_cast<uint32_t>(ticks().count());
	}

	static std::chrono::milliseconds ticks()
	{
		return std::chrono::milliseconds(SDL_GetTicks());
	}

	static uint64_t perfCounter()
	{
		return SDL_GetPerformanceCounter();
	}

	static uint64_t perfFrequency()
	{
		return SDL_GetPerformanceFrequency();
	}

	////////////////////////////////////////////////////////////////////////////

	operator SDL_TimerID() const { return timerId(); }

private:
	SDL_TimerID m_timer = 0;
};

////////////////////////////////////////////////////////////////////////////////

}