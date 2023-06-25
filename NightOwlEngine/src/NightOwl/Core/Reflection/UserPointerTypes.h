#pragma once

#include "TypeDescriptor.h"

namespace NightOwl
{
    class RigidBody2D;

    class RigidBody2DPointerDescriptor : public TypeDescriptor
    {
    public:
        RigidBody2DPointerDescriptor();

        static RigidBody2D GetTarget(const void* pointer);

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override;

        TypeDescriptor* targetType;
    };

    template<>
    inline TypeDescriptor* GetPrimitiveDescriptor<RigidBody2D*>()
    {
        static RigidBody2DPointerDescriptor typeDescriptor;
        return &typeDescriptor;
    }
}
