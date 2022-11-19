#pragma once

#include "TypeDescriptor.h"

namespace NightOwl::Component
{
    class RigidBody2D;
}

namespace NightOwl::Core
{
    class RigidBody2DPointerDescriptor : public TypeDescriptor
    {
    public:
        RigidBody2DPointerDescriptor();

        static Component::RigidBody2D GetTarget(const void* pointer);

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override;

        TypeDescriptor* targetType;
    };

    template<>
    inline TypeDescriptor* GetPrimitiveDescriptor<Component::RigidBody2D*>()
    {
        static RigidBody2DPointerDescriptor typeDescriptor;
        return &typeDescriptor;
    }
}
