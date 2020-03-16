/*
** SDL++, 2020
** Joystick.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"
#include "Haptic.hpp"
#include "Vec2.hpp"

#include <SDL2/SDL_joystick.h>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Joystick
{
	SDL_Joystick *m_joystick = nullptr;
	const bool m_owner = true;

	Joystick(SDL_Joystick *stick, bool owningState)
	: m_joystick(stick)
	, m_owner(owningState)
	{
		assert(!m_owner);
	}

public:
	Joystick()
	: m_owner(false)
	{}

	explicit Joystick(int index)
	: m_joystick(SDL_JoystickOpen(index))
	{
		if (!m_joystick)
			throw Exception("SDL_JoystickOpen");
	}

	explicit Joystick(SDL_Joystick *joystick)
	: m_joystick(joystick)
	{}

	Joystick(Joystick const&) = delete;
	Joystick(Joystick &&other) noexcept
	{
		*this = std::move(other);
	}

	~Joystick()
	{
		if (m_owner && m_joystick)
			SDL_JoystickClose(m_joystick);
	}

	////////////////////////////////////////////////////////////////////////////

	Haptic openHaptic() const
	{
		return Haptic(m_joystick);
	}

	SDL_JoystickPowerLevel powerLevel() const
	{
		const auto power = SDL_JoystickCurrentPowerLevel(m_joystick);
		if (power == SDL_JOYSTICK_POWER_UNKNOWN)
			throw Exception("SDL_JoystickCurrentPowerLevel");
		return power;
	}

	bool attached() const
	{
		return SDL_JoystickGetAttached(m_joystick) == SDL_TRUE;
	}

	int16_t axis(int axis) const
	{
		return SDL_JoystickGetAxis(m_joystick, axis);
	}

	SDL::Vec2i ball(int ball) const
	{
		Vec2i d;
		const int status = SDL_JoystickGetBall(m_joystick, ball, &d.x, &d.y);
		if (status < 0)
			throw Exception("SDL_JoystickGetBall");
		return d;
	}

	uint8_t button(int button) const
	{
		return SDL_JoystickGetButton(m_joystick, button);
	}

	uint8_t hat(int hat) const
	{
		return SDL_JoystickGetHat(m_joystick, hat);
	}

	std::string name() const {
		return {SDL_JoystickName(m_joystick)};
	}

	int hatsCount() const
	{
		const int value = SDL_JoystickNumHats(m_joystick);
		if (value < 0)
			throw Exception("SDL_JoystickNumHats");
		return value;
	}

	int buttonsCount() const
	{
		const int value = SDL_JoystickNumButtons(m_joystick);
		if (value < 0)
			throw Exception("SDL_JoystickNumButtons");
		return value;
	}

	int ballsCount() const
	{
		const int value = SDL_JoystickNumBalls(m_joystick);
		if (value < 0)
			throw Exception("SDL_JoystickNumBalls");
		return value;
	}

	int axesCount() const
	{
		const int value = SDL_JoystickNumAxes(m_joystick);
		if (value < 0)
			throw Exception("SDL_JoystickNumAxes");
		return value;
	}

	SDL_JoystickID id() const
	{
		const auto value = SDL_JoystickInstanceID(m_joystick);
		if (value < 0)
			throw Exception("SDL_JoystickInstanceID");
		return value;
	}

	////////////////////////////////////////////////////////////////////////////

	static Joystick nonOwning(SDL_Joystick *stick)
	{
		return {stick, false};
	}

	static Joystick nonJoystick(SDL_JoystickID id)
	{
		auto object = Joystick(SDL_JoystickFromInstanceID(id), false);
		if (object.m_joystick == nullptr)
			throw Exception("SDL_JoystickFromInstanceID");
		return object;
	}

	////////////////////////////////////////////////////////////////////////////

	Joystick &operator=(const Joystick&) = delete;
	Joystick &operator =(Joystick &&other) noexcept
	{
		if (m_joystick != other.m_joystick)
		{
			m_joystick = other.m_joystick;
			other.m_joystick = nullptr;

			const_cast<bool&>(m_owner) = other.m_owner;
			const_cast<bool&>(other.m_owner) = false;
		}
		return *this;
	}

	bool operator ==(const Joystick &other) const {
		return m_joystick == other.m_joystick;
	}

	bool operator ==(SDL_Joystick *other) const {
		return m_joystick == other;
	}

	bool operator ==(SDL_JoystickID other) const {
		return id() == other;
	}
};

////////////////////////////////////////////////////////////////////////////////

}