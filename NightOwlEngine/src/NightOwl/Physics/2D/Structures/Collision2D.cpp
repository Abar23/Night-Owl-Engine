#include "NightOwlPch.h"

#include "Collision2D.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"

namespace NightOwl
{
	Collision2D::Collision2D(Collider2D * collider, Collider2D * otherCollider)
		:	collider(collider),
			otherCollider(otherCollider)
	{
		body = collider->GetRigidBody();
		otherBody = otherCollider->GetRigidBody();
		object = &collider->GetRigidBody()->GetGameObject();
	}
}
