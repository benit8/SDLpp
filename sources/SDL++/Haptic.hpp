/*
** SDL++, 2020
** Haptic.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL_haptic.h>

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Haptic
{
	SDL_Haptic *m_haptic = nullptr;

	using EffectId = int;
	using EffectList = std::vector<EffectId>;

	EffectList myEffects{};

public:
	SDL_Haptic *ptr() const { return m_haptic; }

	class InstalledEffect
	{
		static constexpr EffectList::size_type invalidIndex = std::numeric_limits<EffectList::size_type>::max();
		EffectList::size_type m_index = invalidIndex;
		Haptic *m_owner = nullptr;
		friend class Haptic;

		void from(InstalledEffect &)
		{}

	public:
		InstalledEffect(EffectList::size_type index, Haptic *owner)
		: m_index{index}
		, m_owner{owner}
		{}

		InstalledEffect() = default;
		InstalledEffect(const InstalledEffect&);
		InstalledEffect &operator=(const InstalledEffect&);

		InstalledEffect(InstalledEffect &&other) noexcept
		{
			*this = std::move(other);
		}

		InstalledEffect &operator=(InstalledEffect &&other) noexcept
		{
			if (m_index != other.m_index) {
				m_index = other.m_index;
				m_owner = other.m_owner;
				other.m_index = invalidIndex;
				other.m_owner = nullptr;
			}
			return *this;
		}

		~InstalledEffect()
		{
			if (m_owner && m_index != invalidIndex) {
				const EffectId myRealId = m_owner->getEffectId(*this);
				SDL_HapticDestroyEffect(m_owner->ptr(), myRealId);
				m_owner->removeEffect(myRealId);
			}
		}

		void run(uint32_t iterations = 1)
		{
			m_owner->runEffect(*this, iterations);
		}
	};

	union Effect
	{
		Uint16              type;      /// Effect type.
		SDL_HapticConstant  constant;  /// Constant effect.
		SDL_HapticPeriodic  periodic;  /// Periodic effect.
		SDL_HapticCondition condition; /// Condition effect.
		SDL_HapticRamp      ramp;      /// Ramp effect.
		SDL_HapticLeftRight leftright; /// Left/Right effect.
		SDL_HapticCustom    custom;    /// Custom effect.

		operator SDL_HapticEffect*() const
		{
			return (SDL_HapticEffect*)(this);
		}

		Effect()
		{
			static_assert(sizeof(Effect::type) == sizeof(SDL_HapticEffect::type), "Please compare the layout between SDL_HapticEffect and sdl::Haptic::Effect");
			static_assert(sizeof(Effect) == sizeof(SDL_HapticEffect), "Please compare the layout between SDL_HapticEffect and sdl::Haptic::Effect");

			SDL_memset(this, 0, sizeof(Effect));
		}
	};

	Haptic()
	{}

	explicit Haptic(int m_hapticindex)
	: m_haptic{SDL_HapticOpen(m_hapticindex)}
	{
		if (!m_haptic)
			throw Exception("SDL_HapticOpen");
	}

	explicit Haptic(SDL_Joystick *joystick)
	: m_haptic{SDL_HapticOpenFromJoystick(joystick)}
	{
		if (!m_haptic)
			throw Exception("SDL_HapticOpenFromJoystick");
	}

	~Haptic()
	{
		if (m_haptic)
			SDL_HapticClose(m_haptic);
	}

	Haptic(Haptic const&) = delete;
	Haptic& operator=(Haptic const&) = delete;

	Haptic(Haptic&& other) noexcept { *this = std::move(other); }
	Haptic& operator=(Haptic&& other) noexcept
	{
		if (m_haptic != other.m_haptic) {
			m_haptic = other.m_haptic;
			myEffects = std::move(other.myEffects);
			other.m_haptic = nullptr;
		}
		return *this;
	}

	bool valid() const { return m_haptic != nullptr; }

	unsigned getCapabilities() const
	{
		if (!valid())
			return 0;

		const auto capabilities = SDL_HapticQuery(m_haptic);

		if (!capabilities)
			throw Exception("SDL_HapticQuery");

		return capabilities;
	}

	bool isCapableOf(int m_hapticflag) const
	{
		return (m_hapticflag & getCapabilities()) != 0;
	}

	InstalledEffect newEffect(const Effect &e)
	{
		if (!isCapableOf(e.type))
			return {};

		const EffectId rawId = SDL_HapticNewEffect(m_haptic, e);
		if (rawId < 0)
			throw Exception("SDL_HapticNewEffect");

		myEffects.push_back(rawId);
		return {myEffects.size() - 1, this};
	}

	EffectList::size_type registeredEffectCount() const
	{
		return myEffects.size();
	}

	EffectId getEffectId(const InstalledEffect &h) const
	{
		return myEffects.at(h.m_index);
	}

	void removeEffect(EffectId e)
	{
		for (size_t i = 0, nbEffects = myEffects.size(); i < nbEffects; ++i) {
			if (myEffects[i] == e)
				myEffects[i] = -1;
		}
	}

	void runEffect(const InstalledEffect &h, uint32_t iterations = 1) const
	{
		const EffectId e = getEffectId(h);
		if (e >= 0 && SDL_HapticRunEffect(m_haptic, getEffectId(h), iterations) < 0)
			throw Exception("SDL_HapticRunEffect");
	}

	bool isEffectCompatible(const Effect &e) const
	{
		return isCapableOf(e.type);
	}
};

////////////////////////////////////////////////////////////////////////////////

}