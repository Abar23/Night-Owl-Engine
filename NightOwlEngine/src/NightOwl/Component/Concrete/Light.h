#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Vec3.h"

namespace NightOwl
{
	class Light : public Component
	{
	public:
		Light();

		float GetRange() const;

		void SetRange(float range);

		float GetIntensity() const;

		void SetIntensity(float intensity);

		Vec3F GetColor() const;

		void SetColor(const Vec3F& color);

	protected:
		void Remove() override;

	private:
		float range;

		float intensity;

		Vec3F color;

		// TODO: add shadow map specific fields
	};
}
