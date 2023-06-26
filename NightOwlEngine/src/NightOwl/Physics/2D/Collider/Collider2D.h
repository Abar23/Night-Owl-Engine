#pragma once

#include "NightOwl/Core/Reflection/Reflection.h"
#include "NightOwl/Math/Vec2.h"

namespace NightOwl
{
	class RigidBody2D;

	class Collider2D
	{
	public:
		Collider2D()
			:	rigidBody2D(nullptr)
		{ }

		virtual ~Collider2D() = default;

		RigidBody2D* GetRigidBody()
		{
			return rigidBody2D;
		}

		Vec2F GetOffsetFromCenterOfObject() const;

		void SetOffsetFromCenterOfObject(const Vec2F& offsetFromCenterOfObject);

		REFLECT()

	protected:
		RigidBody2D* rigidBody2D;

		Vec2F offsetFromCenterOfObject;

		friend class RigidBody2D;
	};
}
