#pragma once

#include "TypeDescriptor.h"
#include "Resolver.h"

namespace NightOwl
{
    template<typename T> class Quaternion;
    template<typename T> class Mat4;
    template<typename T> class Vec4;
    template<typename T> class Vec3;
    template<typename T> class Vec2;

    template <typename T>
    class Vec4Descriptor: public TypeDescriptor
    {
    public:
        Vec4Descriptor();

        void Serialize(const void* object, rapidjson::Document& document, rapidjson::Value& value) override;

        TypeDescriptor* itemType;
    };

    template <typename T>
    class TypeResolver<Vec4<T>>
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
    class TypeResolver<Vec3<T>>
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
    class TypeResolver<Vec2<T>>
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
    class TypeResolver<Mat4<T>>
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
    class TypeResolver<Quaternion<T>>
    {
    public:
        static TypeDescriptor* Get()
        {
            static QuaternionDescriptor<T> typeDescriptor;
            return &typeDescriptor;
        }
    };
}