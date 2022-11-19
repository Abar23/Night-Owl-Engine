#include "UserPointerTypes.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"

namespace NightOwl::Core
{
	RigidBody2DPointerDescriptor::RigidBody2DPointerDescriptor()
		:	TypeDescriptor{ "RigidBody2D*", sizeof(Component::RigidBody2D*) },
			targetType{ TypeResolver<Component::RigidBody2D>::Get() }
	{ }

	Component::RigidBody2D RigidBody2DPointerDescriptor::GetTarget(const void* pointer)
	{
	    const Component::RigidBody2D pointerData = *static_cast<const Component::RigidBody2D*>(pointer);
	    return pointerData;
	}

	void RigidBody2DPointerDescriptor::Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value)
	{
	}
}
