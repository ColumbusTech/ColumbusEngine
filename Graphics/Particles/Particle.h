#pragma once

#include <Math/Vector3.h>
#include <Math/Vector4.h>

namespace Columbus
{

	class Particle
	{
	private:
		size_t i;
	public:
		Vector4 startColor = Vector4(1, 1, 1, 1);
		Vector4 finalColor = Vector4(1, 1, 1, 1);
		Vector4 color = Vector4(1, 1, 1, 1);

		Vector3 velocity = Vector3(0, 1, 0);
		Vector3 startPos = Vector3(0, 0, 0);
		Vector3 startEmitterPos = Vector3(0, 0, 0);
		Vector3 accel = Vector3(0, 0.1, 0);
		Vector3 pos;

		Vector3 startSize = Vector3(1, 1, 1);
		Vector3 finalSize = Vector3(1, 1, 1);
		Vector3 size = Vector3(1, 1, 1);

		float rotation = 0.0;
		float rotationSpeed = 0.0;
		float age = 0.0;
		float TTL = 1.0;
		float cameraDistance = 1000.0;
		float noise[9];

		unsigned int frame = 0;

		void genCircle(const float aRadius, const bool aEmitFromShell)
		{
			float ang = Random::range(0.0, 6.283185306);
			float rad = Random::range(0.0, aRadius);

			if (aEmitFromShell) rad = aRadius;

			startPos.x = rad * cos(ang);
			startPos.y = 0.0;
			startPos.z = rad * sin(ang);
		}

		void genSphere(const float aRadius, const bool aEmitFromShell)
		{
			float rad = Random::range(0.0, aRadius);
			float phi = Random::range(0.0, 6.283185306);
			float tht = Random::range(0.0, 3.141592653);

			if (aEmitFromShell) rad = aRadius;

			startPos.x = rad * cos(phi) * sin(tht);
			startPos.y = rad * sin(phi) * sin(tht);
			startPos.z = rad * cos(tht);
		}

		void genCube(const Vector3 aSize, const bool aEmitFromShell)
		{
			if (aEmitFromShell)
			{
				switch (rand() % 3)
				{
				case 0:
					startPos.x = aSize.x / 2 * ((rand() % 2) ? -1 : 1);
					startPos.y = Random::range(-aSize.y / 2, aSize.y / 2);
					startPos.z = Random::range(-aSize.z / 2, aSize.z / 2);
					break;
				case 1:
					startPos.x = Random::range(-aSize.x / 2, aSize.x / 2);
					startPos.y = aSize.y / 2 * ((rand() % 2) ? -1 : 1);
					startPos.z = Random::range(-aSize.z / 2, aSize.z / 2);
					break;
				case 2:
					startPos.x = Random::range(-aSize.x / 2, aSize.x / 2);
					startPos.y = Random::range(-aSize.y / 2, aSize.y / 2);
					startPos.z = aSize.z / 2 * ((rand() % 2) ? -1 : 1);
					break;
				}
			} else
			{
				startPos.x = Random::range(-aSize.x / 2, aSize.x / 2);
				startPos.y = Random::range(-aSize.y / 2, aSize.y / 2);
				startPos.z = Random::range(-aSize.z / 2, aSize.z / 2);
			}
		}

		void update(const float aTimeTick, const vec3 aCamera)
		{
			age += aTimeTick;

			pos = velocity * age + (accel * 0.5 * age * age);
			pos += startPos + startEmitterPos;

			rotation += rotationSpeed * aTimeTick;
			cameraDistance = pow(aCamera.x - pos.x, 2) + pow(aCamera.y - pos.y, 2) + pow(aCamera.z - pos.z, 2);
		}

		void update(const float aTimeTick, const vec3 aCamera, const vec3 aNoise)
		{
			for (i = 0; i < 9; i++) noise[i] = fmodf(noise[i] + (0.625f * aTimeTick), 256);

			age += aTimeTick;

			pos = velocity * age + (accel * 0.5 * age * age);
			pos += startPos + startEmitterPos;
			pos += aNoise;

			rotation += rotationSpeed * aTimeTick;
			cameraDistance = pow(aCamera.x - pos.x, 2) + pow(aCamera.y - pos.y, 2) + pow(aCamera.z - pos.z, 2);
		}
	};

}





