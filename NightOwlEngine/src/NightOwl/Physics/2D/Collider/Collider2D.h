#pragma once

#include "NightOwl/Core/Reflection/Reflection.h"

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

		REFLECT()

	protected:
		RigidBody2D* rigidBody2D;

		friend class RigidBody2D;
	};
}
