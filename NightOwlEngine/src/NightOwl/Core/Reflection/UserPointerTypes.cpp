#include "UserPointerTypes.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"

namespace NightOwl
{
	RigidBody2DPointerDescriptor::RigidBody2DPointerDescriptor()
		:	TypeDescriptor{ "RigidBody2D*", sizeof(RigidBody2D*) },
			targetType{ TypeResolver<RigidBody2D>::Get() }
	{ }

	RigidBody2D RigidBody2DPointerDescriptor::GetTarget(const void* pointer)
	{
	    const RigidBody2D pointerData = *static_cast<const RigidBody2D*>(pointer);
	    return pointerData;
	}

	void RigidBody2DPointerDescriptor::Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value)
	{
	}
}
