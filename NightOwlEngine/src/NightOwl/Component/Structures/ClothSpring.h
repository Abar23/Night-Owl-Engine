#pragma once

namespace NightOwl
{
	enum class SpringType
	{
		Structural,
		Shear,
		Flexion
	};

	class ClothParticle;

	class ClothSpring
	{
	public:
		ClothSpring(ClothParticle* particle, ClothParticle* otherParticle, float rest, SpringType type)
			: particle(particle), otherParticle(otherParticle), restLength(rest), type(type)
		{}

		ClothParticle* particle;

		ClothParticle* otherParticle;

		float restLength;

		SpringType type;
	};
}
