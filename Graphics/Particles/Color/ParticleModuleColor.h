#pragma once

#include <Graphics/Particles/Color/ParticleModuleColorBase.h>

namespace Columbus
{

	class ParticleModuleColor : public ParticleModuleColorBase
	{
	public:
		Vector4 Min;
		Vector4 Max;

		ParticleModuleColor() :
			Min(Vector4(1, 1, 1, 1)),
			Max(Vector4(1, 1, 1, 1)) {}
		/*
		* For determening module type
		*/
		Type GetType() const override { return Type::Color; }
		/*
		* Set parameter for particle
		*/
		void Spawn(Particle& OutParticle) override
		{
			OutParticle.color = Vector4::random(Min, Max);
		}
		/*
		* Update particle parameter
		*/
		void Update(Particle& OutParticle) override {}

		~ParticleModuleColor() override {}
	};

}





