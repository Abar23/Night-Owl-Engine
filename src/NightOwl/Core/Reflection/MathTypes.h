#pragma once

#include "TypeDescriptor.h"
#include "Resolver.h"

namespace NightOwl::Math
{
    template<typename T> class Quaternion;
    template<typename T> class Mat4;
    template<typename T> class Vec4;
    template<typename T> class Vec3;
    template<typename T> class Vec2;
}

namespace NightOwl::Core
{
    template <typename T>
    class Vec4Descriptor: public TypeDescriptor
    {
    public:
        Vec4Descriptor();

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override;

        TypeDescriptor* itemType;
    };

    template <typename T>
    class TypeResolver<Math::Vec4<T>>
    {
    public:
        static TypeDescriptor* Get()
        {
            static Vec4Descriptor<T> typeDescriptor;
            return &typeDescriptor;
        }
    };


    template <typename T>
    class Vec3Descriptor : public TypeDescriptor
    {
    public:
        Vec3Descriptor();

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override;

        TypeDescriptor* itemType;
    };

    template <typename T>
    class TypeResolver<Math::Vec3<T>>
    {
    public:
        static TypeDescriptor* Get()
        {
            static Vec3Descriptor<T> typeDescriptor;
            return &typeDescriptor;
        }
    };


    template <typename T>
    class Vec2Descriptor : public TypeDescriptor
    {
    public:
        Vec2Descriptor();

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override;

        TypeDescriptor* itemType;
    };

    template <typename T>
    class TypeResolver<Math::Vec2<T>>
    {
    public:
        static TypeDescriptor* Get()
        {
            static Vec2Descriptor<T> typeDescriptor;
            return &typeDescriptor;
        }
    };


    template <typename T>
    class Mat4Descriptor : public TypeDescriptor
    {
    public:
        Mat4Descriptor();

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override;

        TypeDescriptor* itemType;
    };

    template <typename T>
    class TypeResolver<Math::Mat4<T>>
    {
    public:
        static TypeDescriptor* Get()
        {
            static Mat4Descriptor<T> typeDescriptor;
            return &typeDescriptor;
        }
    };


    template <typename T>
    class QuaternionDescriptor : public TypeDescriptor
    {
    public:
        QuaternionDescriptor();

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override;

        TypeDescriptor* itemType;
    };

    template <typename T>
    class TypeResolver<Math::Quaternion<T>>
    {
    public:
        static TypeDescriptor* Get()
        {
            static QuaternionDescriptor<T> typeDescriptor;
            return &typeDescriptor;
        }
    };
}