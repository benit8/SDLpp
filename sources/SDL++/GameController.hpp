/*
** SDL++, 2020
** GameController.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"
#include "Haptic.hpp"

#include <SDL2/SDL_gamecontroller.h>

#include <cassert>
#include <chrono>
#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class GameController
{
public:
	GameController() = default;

	explicit GameController(int joystickIndex)
	: m_controller(SDL_GameControllerOpen(joystickIndex))
	{
		if (!m_controller)
			throw Exception("SDL_GameControllerOpen");
	}

	explicit GameController(SDL_GameController *controller)
	: m_controller(controller)
	{}

	GameController(const GameController&) = delete;

	GameController(GameController &&other) noexcept
	{
		*this = std::move(other);
	}

	~GameController()
	{
		if (m_owned && m_controller)
			SDL_GameControllerClose(m_controller);
	}

	////////////////////////////////////////////////////////////////////////////

	SDL_GameController *ptr() const { return m_controller; }

	Haptic openHaptic() const
	{
		return Haptic(SDL_GameControllerGetJoystick(m_controller));
	}

	bool isAttached() const
	{
		return SDL_GameControllerGetAttached(m_controller) == SDL_TRUE;
	}

	int16_t getAxis(SDL_GameControllerAxis axis) const
	{
		return SDL_GameControllerGetAxis(m_controller, axis);
	}

	int8_t getButton(SDL_GameControllerButton button) const
	{
		return SDL_GameControllerGetButton(m_controller, button);
	}

#if SDL_VERSION_ATLEAST(2, 0, 9)
	int rumble(uint16_t lowFreq, uint16_t highFreq, std::chrono::milliseconds duration) const
	{
		return rumble(lowFreq, highFreq, static_cast<uint32_t>(duration.count()));
	}

	int rumble(uint16_t lowFreq, uint16_t highFreq, uint32_t millisecDuration) const
	{
		return SDL_GameControllerRumble(m_controller, lowFreq, highFreq, millisecDuration);
	}
#endif

	std::string name() const
	{
		if (!m_controller)
			return {};
		return {SDL_GameControllerName(m_controller)};
	}

	////////////////////////////////////////////////////////////////////////////

	static std::string getControllerName(int joystick_index)
	{
		const char *n = SDL_GameControllerNameForIndex(joystick_index);
		if (!n)
			throw Exception("SDL_GameControllerNameForIndex");
		return {n};
	}

	// static std::string getAxisName(SDL_GameControllerAxis axis)
	//{
	//	// TODO: error when SDL return null
	//	return { SDL_GameControllerGetStringForAxis(axis) };
	//}

	// static std::string getButtonName(SDL_GameControllerButton button)
	//{
	//	// TODO: error when SDL return null
	//	return { SDL_GameControllerGetStringForButton(button) };
	//}

	static int loadMappingDatabase(const std::string &filePath)
	{
		return loadMappingDatabase(filePath.c_str());
	}

	///Load a file database
	static int loadMappingDatabase(const char *filePath)
	{
		const auto state = SDL_GameControllerAddMappingsFromFile(filePath);
		if (state < 0)
			throw Exception("SDL_GameControllerAddMappingsFromFile");
		return state;
	}

	static int addMapping(const std::string &mappingString)
	{
		return addMapping(mappingString.c_str());
	}

	static int addMapping(const char *mappingString)
	{
		const auto state = SDL_GameControllerAddMapping(mappingString);
		if (state < 0)
			throw Exception("SDL_GameControllerAddMapping");
		return state;
	}

	static std::vector<GameController> openAllAvailableControllers()
	{
		std::vector<GameController> controllers;

		for (int nb_sticks = SDL_NumJoysticks(), i = 0; i < nb_sticks; ++i) {
			if (SDL_IsGameController(i)) {
				try {
					controllers.emplace_back(i);
				}
				catch (const SDL::Exception&) {
					continue;
				}
			}
		}

		return controllers;
	}

	static GameController nonOwning(SDL_JoystickID joystickId)
	{
		return {SDL_GameControllerFromInstanceID(joystickId), false};
	}

	static GameController nonOwning(SDL_GameController* controller)
	{
		return {controller, false};
	}

	////////////////////////////////////////////////////////////////////////////

	GameController &operator =(const GameController&) = delete;

	GameController &operator=(GameController &&other) noexcept
	{
		if (m_controller != other.m_controller) {
			m_controller = other.ptr();
			m_owned = other.m_owned;
			other.m_owned = false;
			other.m_controller = nullptr;
		}
		return *this;
	}

private:
	GameController(SDL_GameController *controller, bool nonOwned)
	: m_controller(controller)
	, m_owned(nonOwned)
	{
		assert(!m_owned);
	}

	SDL_GameController *m_controller = nullptr;
	bool m_owned = true;
};

////////////////////////////////////////////////////////////////////////////////

}