#pragma once

#include "Collider2D.h"
#include "NightOwl/Math/Vec2.h"
#include "NightOwl/Core/Reflection/Reflection.h"

namespace NightOwl
{
	class CircleCollider2D : public Collider2D
	{
	public:
		CircleCollider2D() = delete;

		CircleCollider2D(const Vec2F& offsetFromCenterOfObject, float radius);

		void SetRadius(float radius);

		float GetRadius() const;

		Vec2F GetCenterOfCircle() const;

		REFLECT()

	private:
		Vec2F offsetFromCenterOfObject;

		float radius;
	};
}
